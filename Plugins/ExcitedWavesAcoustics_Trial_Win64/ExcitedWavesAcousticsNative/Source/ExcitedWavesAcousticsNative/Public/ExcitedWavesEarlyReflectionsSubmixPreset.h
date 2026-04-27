// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "HAL/CriticalSection.h"
#include "Sound/SoundEffectSubmix.h"
#include "Templates/SharedPointer.h"
#include <atomic>
#include "ExcitedWavesEarlyReflectionsSubmixPreset.generated.h"

struct FERImageSourceData
{
	FVector Position = FVector::ZeroVector;
	FVector WallHitPoint = FVector::ZeroVector;
	uint32 SourceId = 0;
	float Level = 0.0f;
	float Absorption = 0.0f;
};

struct FERSceneUpdate
{
	TArray<FERImageSourceData> Sources;
	FVector EmitterPosition = FVector::ZeroVector;
	float MasterGain = 1.0f;
	float DistanceScalingFactor = 1.0f;
	bool bForceKill = false;
};

USTRUCT(BlueprintType)
struct FExcitedWavesEarlyReflectionsSubmixSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Early Reflections|Effect", meta = (DisplayName = "Max Delay Ms", ClampMin = "10.0", ClampMax = "200.0", ToolTip = "Maximum delay line length (ms). Reflection taps are clamped to this budget; increase for larger spaces, at higher CPU and memory cost."))
	float MaxDelayMs = 120.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Early Reflections|Effect", meta = (DisplayName = "Max Taps", ClampMin = "1", ClampMax = "8", ToolTip = "How many image sources are mixed: strongest Side rays first, up to this count. Cannot exceed the internal limit of 8 taps per instance."))
	int32 MaxTaps = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Early Reflections|Effect", meta = (DisplayName = "Speed Of Sound Cm Per Sec", ClampMin = "1000.0", ClampMax = "50000.0", ToolTip = "Speed of sound in cm/s. Converts geometric path length to delay in samples for each tap."))
	float SpeedOfSoundCmPerSec = 34300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Early Reflections|Effect", meta = (DisplayName = "Lowpass Min Cutoff Hz", ClampMin = "200.0", ClampMax = "20000.0", ToolTip = "Low-pass cutoff (Hz) at full absorption (most damped reflections)."))
	float LowpassMinCutoffHz = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Early Reflections|Effect", meta = (DisplayName = "Lowpass Max Cutoff Hz", ClampMin = "200.0", ClampMax = "20000.0", ToolTip = "Low-pass cutoff (Hz) at zero absorption (bright reflections). Interpolated between min and max from per-ray absorption."))
	float LowpassMaxCutoffHz = 16000.0f;
};

struct FERSharedState
{
	FCriticalSection PendingSceneCS;
	FCriticalSection SettingsCS;
	FExcitedWavesEarlyReflectionsSubmixSettings Settings;
	TArray<FERSceneUpdate> PendingScenes;
	std::atomic<bool> bSettingsDirty{ true };
};

class FSubmixEffectExcitedWavesEarlyReflections;

namespace ExcitedWaves::Native::ER
{
	// Publishes ER tap and memory (KB) counters into UE stats. Call from the
	// game thread only, e.g. a low-rate ticker. ER CPU time is a separate
	// cycle stat (Per-Detector Frame (Total) in EWA_ER), scoped in the audio
	// process path. Do not call SET_*_STAT from the audio render buffer loop.
	EXCITEDWAVESACOUSTICSNATIVE_API void PublishStatsToUE();
}

UCLASS()
class EXCITEDWAVESACOUSTICSNATIVE_API USubmixEffectExcitedWavesEarlyReflectionsPreset : public USoundEffectSubmixPreset
{
	GENERATED_BODY()

	friend class FSubmixEffectExcitedWavesEarlyReflections;

public:
	USubmixEffectExcitedWavesEarlyReflectionsPreset();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Early Reflections|Effect", meta = (ShowOnlyInnerProperties, ToolTip = "Early reflection delay network: tap count, speed of sound, and absorption-driven low-pass."))
	FExcitedWavesEarlyReflectionsSubmixSettings Settings;

	virtual FSoundEffectBase* CreateNewEffect() const override;
	virtual USoundEffectPreset* CreateNewPreset(UObject* InParent, FName Name, EObjectFlags Flags) const override;
	virtual UClass* GetSupportedClass() const override;
	virtual FText GetAssetActionName() const override;

	virtual void Init() override;
	virtual void PostInitProperties() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	void PushSceneUpdate(const FERSceneUpdate& InScene);

	UFUNCTION(BlueprintCallable, Category = "Early Reflections|Effect")
	void SyncSettingsToAudio();

	TSharedPtr<FERSharedState, ESPMode::ThreadSafe> GetSharedState() const { return SharedState; }

private:

	void EnsureSharedState();
	void PublishSettingsToSharedState();

	TSharedPtr<FERSharedState, ESPMode::ThreadSafe> SharedState;
};
