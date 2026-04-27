// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "ExcitedWavesAcousticsSDK/Public/AcousticsTypes.h"
#include "Templates/SharedPointer.h"

class AActor;
class UAcousticsSubsystem;
class UAudioComponent;
class UWorld;

struct FAcousticsSendGroup
{
	float Close = 0.0f;
	float Mid = 0.0f;
	float Far = 0.0f;
	float Outdoor = 0.0f;
	float MeanAbsorption = 0.0f;
	float PresetAbsorption = 0.0f;
	float CloseAbsorption = 0.0f;
	float MidAbsorption = 0.0f;
	float FarAbsorption = 0.0f;
	float OutdoorAbsorption = 0.0f;
};

struct FAcousticsReverbSendData
{
	FAcousticsSendGroup Emitter;
	FAcousticsSendGroup CrossRoom;
	float DistanceToListener = 0.0f;
	float ReverbMultiplier = 1.0f;
	float CrossRoomContribution = 0.0f;
};

struct FAcousticsReflectUpdateParams
{
	EWAcoustics::SDK::DetectorId DetectorId = 0;
	UAcousticsSubsystem* Subsystem = nullptr;
	UWorld* World = nullptr;
	float DistanceScalingFactor = 1.0f;
	float Level = 1.0f;
	bool bIgnoreInDebugDraw = false;
	bool bShowReflectDebug = false;
	FColor ReflectDebugColor = FColor(0, 240, 255);
	FVector ViewerPosition = FVector::ZeroVector;
};

class EXCITEDWAVESACOUSTICS_API IAcousticsAudioAdapter
{
public:
	virtual ~IAcousticsAudioAdapter() = default;

	// Standard init: adapter resolves the underlying audio handle (e.g. UAudioComponent) from Owner.
	virtual bool Initialize(AActor* Owner) = 0;

	// One-shot init: caller provides an explicit audio handle (spawned via SpawnSoundAtLocation or
	// similar). Adapters should prefer ExplicitAudioComponent over searching Owner. Default
	// implementation falls back to Initialize(Owner) so legacy adapters keep compiling.
	virtual bool InitializeWithExplicitAudio(AActor* Owner, UAudioComponent* ExplicitAudioComponent)
	{
		return Initialize(Owner);
	}

	virtual void Shutdown() = 0;

	virtual void ApplyReverbSends(const FAcousticsReverbSendData& Data) = 0;
	virtual void ClearReverbSends() = 0;

	virtual void UpdateReflections(const FAcousticsReflectUpdateParams& Params) = 0;
	virtual void ClearReflections() = 0;

	virtual float GetLastReflectUpdateTime() const = 0;
	virtual FVector GetLastReflectUpdatePosition() const = 0;
	virtual void SetLastReflectUpdateTime(float Time) = 0;
	virtual void SetLastReflectUpdatePosition(const FVector& Pos) = 0;

	virtual void SetEnableReflections(bool bEnable) = 0;
	virtual bool IsReflectionsEnabled() const = 0;

	virtual float GetAcousticsSendsUpdateIntervalSeconds() const { return 0.05f; }

	virtual bool IsAudioSourceActive() const { return true; }

#if !UE_BUILD_SHIPPING
	virtual void GatherSubmixSendDebugLines(TArray<FString>& OutLines) const { OutLines.Reset(); }
#endif
};

class EXCITEDWAVESACOUSTICS_API FAcousticsAudioAdapterRegistry
{
public:
	using FactoryFunc = TFunction<TSharedPtr<IAcousticsAudioAdapter>()>;

	static FAcousticsAudioAdapterRegistry& Get();

	void RegisterFactory(FName AdapterName, FactoryFunc Factory);
	void UnregisterFactory(FName AdapterName);

	TSharedPtr<IAcousticsAudioAdapter> CreateAdapter() const;

	bool HasRegisteredAdapters() const { return Factories.Num() > 0; }

private:
	TMap<FName, FactoryFunc> Factories;
};
