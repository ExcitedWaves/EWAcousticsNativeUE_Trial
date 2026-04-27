// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ExcitedWavesEarlyReflectionsSubmixPreset.h"
#include "Sound/SoundSubmix.h"
#include "ExcitedWavesNativeAcousticsSettings.generated.h"

USTRUCT()
struct EXCITEDWAVESACOUSTICSNATIVE_API FNativeReverbPreset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Config, Category = "Late Reverbs|Reverb Preset", meta = (ClampMin = "0.0", ClampMax = "1.0", ToolTip = "Upper bound of mean surface absorption for this row. Presets are sorted by Max Absorption; the adapter picks the first preset whose bound covers the current absorption (with hysteresis)."))
	float MaxAbsorption = 1.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Late Reverbs|Reverb Preset", meta = (DisplayName = "Send Volume Offset (dB)", ClampMin = "-24.0", ClampMax = "24.0", ToolTip = "Row makeup in dB for this preset, applied to all late bins while building per-submix levels (before Global Send Volume Offset). When 0, the adapter averages the per-bus Send Volume Offsets below for one combined row gain (legacy tuning)."))
	float SendVolumeOffsetDb = 0.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Late Reverbs|Reverb Preset", meta = (DisplayName = "Close Reverb Submix", ToolTip = "Submix used for the Close reverb bin when this preset is active."))
	TSoftObjectPtr<USoundSubmix> CloseReverbSubmix;

	UPROPERTY(EditAnywhere, Config, Category = "Late Reverbs|Reverb Preset", meta = (DisplayName = "Send Volume Offset (dB)", ClampMin = "-24.0", ClampMax = "24.0", ToolTip = "Per-bus trim in dB. When Send Volume Offset above is 0, the native adapter averages Close/Mid/Far/Outdoor offsets into one row gain (legacy). When Send Volume Offset is non-zero, per-bus offsets are ignored for gain."))
	float CloseSendVolumeOffsetDb = 0.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Late Reverbs|Reverb Preset", meta = (DisplayName = "Mid Reverb Submix", ToolTip = "Submix used for the Mid reverb bin when this preset is active."))
	TSoftObjectPtr<USoundSubmix> MidReverbSubmix;

	UPROPERTY(EditAnywhere, Config, Category = "Late Reverbs|Reverb Preset", meta = (DisplayName = "Send Volume Offset (dB)", ClampMin = "-24.0", ClampMax = "24.0", ToolTip = "Per-bus trim in dB; see Close row tooltip."))
	float MidSendVolumeOffsetDb = 0.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Late Reverbs|Reverb Preset", meta = (DisplayName = "Far Reverb Submix", ToolTip = "Submix used for the Far reverb bin when this preset is active."))
	TSoftObjectPtr<USoundSubmix> FarReverbSubmix;

	UPROPERTY(EditAnywhere, Config, Category = "Late Reverbs|Reverb Preset", meta = (DisplayName = "Send Volume Offset (dB)", ClampMin = "-24.0", ClampMax = "24.0", ToolTip = "Per-bus trim in dB; see Close row tooltip."))
	float FarSendVolumeOffsetDb = 0.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Late Reverbs|Reverb Preset", meta = (DisplayName = "Outdoor Reverb Submix", ToolTip = "Submix used for the Outdoor reverb bin when this preset is active."))
	TSoftObjectPtr<USoundSubmix> OutdoorReverbSubmix;

	UPROPERTY(EditAnywhere, Config, Category = "Late Reverbs|Reverb Preset", meta = (DisplayName = "Send Volume Offset (dB)", ClampMin = "-24.0", ClampMax = "24.0", ToolTip = "Per-bus trim in dB; see Close row tooltip."))
	float OutdoorSendVolumeOffsetDb = 0.0f;
};

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Excited Waves Acoustics Native Audio"))
class EXCITEDWAVESACOUSTICSNATIVE_API UExcitedWavesNativeAcousticsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UExcitedWavesNativeAcousticsSettings();

	virtual FName GetCategoryName() const override;
#if WITH_EDITOR
	virtual FText GetSectionDescription() const override;
#endif

	UFUNCTION(BlueprintCallable, Category = "Native Acoustics Settings")
	static const UExcitedWavesNativeAcousticsSettings* Get();

	UPROPERTY(EditAnywhere, Config, Category = "Late Reverbs|Reverb Presets", meta = (DisplayName = "Reverb Presets", ToolTip = "Rows sorted by Max Absorption ascending. Each row maps four reverb submixes (Close/Mid/Far/Outdoor). The adapter selects a row from measured absorption and crossfades on changes. Per frame, unique submix send targets are aggregated up to 32; beyond that, additional sends may be dropped."))
	TArray<FNativeReverbPreset> ReverbPresets;

	UPROPERTY(EditAnywhere, Config, Category = "Late Reverbs|Reverb Presets", meta = (DisplayName = "Preset Crossfade Duration (s)", ClampMin = "0.0", ClampMax = "2.0", ToolTip = "Seconds to crossfade reverb sends when the active preset row changes."))
	float PresetCrossfadeDuration = 0.3f;

	UPROPERTY(EditAnywhere, Config, Category = "Late Reverbs|Levels", meta = (DisplayName = "Global Send Volume Offset (dB)", ClampMin = "-24.0", ClampMax = "24.0", ToolTip = "Makeup gain in dB applied once per late-reverb submix after emitter bins, cross-room, and preset row gain are combined. Cross-room and direct-to-reverb ratio (distance to listener) are already baked into the send weights from the SDK before this step."))
	float ReverbSendVolumeOffsetDb = 8.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Late Reverbs|Levels", meta = (DisplayName = "Clamp Send Levels", ToolTip = "When enabled, each bin send is clamped to 0–1 after scaling. Disable only if you intentionally drive sends above unity."))
	bool bClampSendLevels = false;

	UPROPERTY(EditAnywhere, Config, Category = "Late Reverbs|Send Updates", meta = (DisplayName = "Acoustics Sends Update Interval (s)", ClampMin = "0.01", ClampMax = "2.0", ToolTip = "Minimum time between late reverb send updates on UAcousticsDetectorComponent when using the native audio adapter."))
	float AcousticsSendsUpdateIntervalSeconds = 0.01f;

	UPROPERTY(EditAnywhere, Config, Category = "Late Reverbs|Send Updates", meta = (DisplayName = "Submix Send Min Level Delta", ClampMin = "0.0", ClampMax = "0.2", ToolTip = "Skip SetSubmixSend when the linear send level changes by less than this. 0 applies every update."))
	float SubmixSendMinLevelDelta = 0.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Early Reflections|General", meta = (DisplayName = "Enable Early Reflections", ToolTip = "When enabled, image-source reflections are computed for this integration and the ER submix effect receives scene updates."))
	bool bEnableEarlyReflections = true;

	UPROPERTY(EditAnywhere, Config, Category = "Early Reflections|General", meta = (DisplayName = "Early Reflection Send Level", ClampMin = "0.0", ClampMax = "2.0", ToolTip = "Submix send amount from the audio component into the Early Reflections submix (Unreal SetSubmixSend). Controls how much of the source signal feeds the ER chain."))
	float EarlyReflectionSendLevel = 0.9f;

	UPROPERTY(EditAnywhere, Config, Category = "Early Reflections|General", meta = (DisplayName = "ER Output Submix", ToolTip = "Submix the ER bus is dynamically connected to (e.g. for monitoring or downstream processing). If unset, falls back to the project Master Submix."))
	TSoftObjectPtr<USoundSubmix> EarlyReflectionOutputSubmix;

	UPROPERTY(EditAnywhere, Config, Category = "Early Reflections|General", meta = (DisplayName = "Show ER Absorption Labels", ToolTip = "When enabled, draws absorption labels for ER debug visualization (editor/development builds)."))
	bool bShowERAbsorptionLabels = false;

	UPROPERTY(EditAnywhere, Config, Category = "Early Reflections|Effect", meta = (ShowOnlyInnerProperties, ToolTip = "Delay-line and mixing parameters for the Early Reflections submix effect (same fields as the Excited Waves Early Reflections submix preset asset)."))
	FExcitedWavesEarlyReflectionsSubmixSettings EarlyReflectionEffect;
};
