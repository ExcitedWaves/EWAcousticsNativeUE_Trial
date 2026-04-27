// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ExcitedWavesAcousticsSDK/Public/AcousticsDetectorManager.h"
#include "ExcitedWavesAcousticsSDK/Public/AcousticsTypes.h"
#include "ExcitedWavesAcousticsSDK/Public/AcousticsDebugTypes.h"
#include "ExcitedWavesAcousticsSettings.h"
#include "AcousticsDetectorRegistration.h"
#include "AcousticsBPProcessedSends.h"
#include "IAcousticsAudioAdapter.h"
#include "Sound/SoundSubmix.h"
#include "CollisionQueryParams.h"
#include "Engine/World.h"
#include "WorldCollision.h"
#include "Containers/Ticker.h"
#include "Templates/SharedPointer.h"
#include <memory>
#include <vector>
#include "AcousticsSubsystem.generated.h"

class USoundBase;
class USoundAttenuation;
class USoundConcurrency;
class UAudioComponent;
class AAcousticsOneShotCarrier;

DECLARE_STATS_GROUP(TEXT("EWA"), STATGROUP_ewa, STATCAT_Advanced);

DECLARE_CYCLE_STAT_EXTERN(TEXT("Subsystem Tick"), STAT_EWA_SubsystemTick, STATGROUP_ewa, EXCITEDWAVESACOUSTICS_API);
DECLARE_CYCLE_STAT_EXTERN(TEXT("Per-Detector Frame (All)"), STAT_EWA_PerDetectorFrameAll, STATGROUP_ewa, EXCITEDWAVESACOUSTICS_API);
DECLARE_CYCLE_STAT_EXTERN(TEXT("Update Detector"), STAT_EWA_UpdateDetector, STATGROUP_ewa, EXCITEDWAVESACOUSTICS_API);
DECLARE_CYCLE_STAT_EXTERN(TEXT("Schedule Raycasts"), STAT_EWA_ScheduleRaycasts, STATGROUP_ewa, EXCITEDWAVESACOUSTICS_API);
DECLARE_CYCLE_STAT_EXTERN(TEXT("Raycast Complete"), STAT_EWA_RaycastComplete, STATGROUP_ewa, EXCITEDWAVESACOUSTICS_API);
DECLARE_CYCLE_STAT_EXTERN(TEXT("Immediate Detection"), STAT_EWA_ImmediateDetection, STATGROUP_ewa, EXCITEDWAVESACOUSTICS_API);
DECLARE_CYCLE_STAT_EXTERN(TEXT("Update Sends"), STAT_EWA_UpdateSends, STATGROUP_ewa, EXCITEDWAVESACOUSTICS_API);

DECLARE_DWORD_ACCUMULATOR_STAT_EXTERN(TEXT("Total Detectors"), STAT_EWA_TotalDetectors, STATGROUP_ewa, EXCITEDWAVESACOUSTICS_API);
DECLARE_DWORD_ACCUMULATOR_STAT_EXTERN(TEXT("Active Detectors"), STAT_EWA_ActiveDetectors, STATGROUP_ewa, EXCITEDWAVESACOUSTICS_API);
DECLARE_DWORD_ACCUMULATOR_STAT_EXTERN(TEXT("Active (Analysis)"), STAT_EWA_ActiveAnalysisCount, STATGROUP_ewa, EXCITEDWAVESACOUSTICS_API);
DECLARE_DWORD_ACCUMULATOR_STAT_EXTERN(TEXT("Pending Raycasts"), STAT_EWA_PendingRaycasts, STATGROUP_ewa, EXCITEDWAVESACOUSTICS_API);
DECLARE_DWORD_ACCUMULATOR_STAT_EXTERN(TEXT("Memory (KB)"), STAT_EWA_MemoryKB, STATGROUP_ewa, EXCITEDWAVESACOUSTICS_API);

class UAcousticsDetectorComponent;
class AActor;

DECLARE_MULTICAST_DELEGATE_OneParam(FAcousticsPreSubsystemTickDelegate, UAcousticsSubsystem*);

USTRUCT(BlueprintType)
struct FAcousticsDebugOverlayEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Acoustics")
	FString ActorName;

	UPROPERTY(BlueprintReadOnly, Category = "Acoustics")
	FString Text;

	UPROPERTY(BlueprintReadOnly, Category = "Acoustics")
	FLinearColor TextColor = FLinearColor::Red;
};

UCLASS()
class EXCITEDWAVESACOUSTICS_API UAcousticsSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	UAcousticsSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override { return true; }

	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override;
	virtual bool IsTickableInEditor() const override { return false; }
	virtual bool IsTickableWhenPaused() const override { return false; }

	EWAcoustics::SDK::DetectorId RegisterComponent(UAcousticsDetectorComponent* Component);
	void UnregisterComponent(EWAcoustics::SDK::DetectorId DetectorId);

	EWAcoustics::SDK::DetectorId RegisterExternalDetector(const FAcousticsExternalDetectorParams& Params);
	void UnregisterDetector(EWAcoustics::SDK::DetectorId DetectorId);
	void SetExternalDetectorWorldLocation(EWAcoustics::SDK::DetectorId DetectorId, const FVector& WorldLocation);
	void SetExternalDetectorWorldLocations(const TArray<TPair<EWAcoustics::SDK::DetectorId, FVector>>& Pairs);

	FAcousticsPreSubsystemTickDelegate OnPreSubsystemTick;

	UFUNCTION(BlueprintCallable, Category = "Acoustics|Listener")
	void SetListenerPosition(const FVector& Position);

	void UpdateDetector(EWAcoustics::SDK::DetectorId DetectorId, const EWAcoustics::SDK::Vector3& Position, float DeltaTime);
	void ForceRestartDetector(EWAcoustics::SDK::DetectorId DetectorId);

	const EWAcoustics::SDK::AcousticsResults* GetDetectorResults(EWAcoustics::SDK::DetectorId DetectorId) const;
	const EWAcoustics::SDK::DebugData* GetDetectorDebugData(EWAcoustics::SDK::DetectorId DetectorId) const;
	bool DetectorHasNewDebugData(EWAcoustics::SDK::DetectorId DetectorId) const;
	void MarkDetectorDebugDataConsumed(EWAcoustics::SDK::DetectorId DetectorId);

	const std::vector<EWAcoustics::SDK::ProbeResult>* GetDetectorSideProbeResults(EWAcoustics::SDK::DetectorId DetectorId) const;
	const std::vector<EWAcoustics::SDK::ProbeResult>* GetDetectorUpProbeResults(EWAcoustics::SDK::DetectorId DetectorId) const;
	const std::vector<EWAcoustics::SDK::ImageSource>* GetDetectorImageSources(EWAcoustics::SDK::DetectorId DetectorId) const;
	bool IsDetectorReflectCulled(EWAcoustics::SDK::DetectorId DetectorId) const;

	uint32 GetGlobalRaysScheduledThisFrame() const;
	int32 GetPendingRaycastCount() const { return m_pendingRaycasts.Num(); }
	const EWAcoustics::SDK::AcousticsConfig& GetGlobalConfig() const { return m_globalConfig; }
	const FVector& GetListenerPosition() const { return m_listenerPosition; }

	float GetCachedEmitterListenerDistanceCm(EWAcoustics::SDK::DetectorId DetectorId) const;

	const FVector& GetListenerForward() const { return m_listenerForward; }

	const FVector& GetListenerUp() const { return m_listenerUp; }

	void SetListenerDetector(EWAcoustics::SDK::DetectorId DetectorId);
	EWAcoustics::SDK::DetectorId GetListenerDetectorId() const { return m_listenerDetectorId; }
	const EWAcoustics::SDK::AcousticsResults* GetListenerDetectorResults() const;

	void SetListenerOrientation(const FVector& Forward, const FVector& Up);

	/** Returns processed late-reverb send levels: cross-room blend, enclosure scaling, and direct-to-reverb ratio vs listener distance applied in the SDK. */
	EWAcoustics::SDK::ProcessedSendLevels GetDetectorProcessedSends(EWAcoustics::SDK::DetectorId DetectorId) const;

	UFUNCTION(BlueprintPure, Category = "Acoustics|Analysis",
		meta = (DisplayName = "Get Detector Processed Sends", CompactNodeTitle = "Get Proc. Sends", Keywords = "reverb send processed GetDetectorProcessedSends"))
	FAcousticsBPProcessedSendLevels GetDetectorProcessedSendsFromComponent(const UAcousticsDetectorComponent* Detector) const;

	void SyncListenerBlendConfig();

	/** Debug overlay entries for emitters with "Draw Debug Text On Screen" enabled. Bind to UMG widget. */
	UFUNCTION(BlueprintCallable, Category = "Acoustics|Debug", meta = (DisplayName = "Get Debug Overlay Texts"))
	TArray<FAcousticsDebugOverlayEntry> GetDebugOverlayTexts() const;

	void SetDebugOverlayText(EWAcoustics::SDK::DetectorId DetectorId, const FString& ActorName, const FString& Text, FLinearColor TextColor = FLinearColor::Red);
	void ClearDebugOverlayText(EWAcoustics::SDK::DetectorId DetectorId);

	/** Disabling clears all sends; enabling triggers re-evaluation. */
	UFUNCTION(BlueprintCallable, Category = "Acoustics")
	void SetSystemEnabled(bool bEnabled);

	UFUNCTION(BlueprintPure, Category = "Acoustics")
	bool IsSystemEnabled() const { return m_bSystemEnabled; }

	UFUNCTION(BlueprintCallable, Category = "Acoustics|Debug")
	void SetDebugTextEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Acoustics|Debug")
	void SetDebugRaysEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Acoustics|Debug")
	void SetDebugHitsEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Acoustics|Debug", meta = (DisplayName = "Set Show Detector Positions"))
	void SetDebugDetectorPositionsEnabled(bool bEnabled);

	UFUNCTION(BlueprintPure, Category = "Acoustics|Debug")
	bool IsDebugTextEnabled() const { return m_bDebugTextEnabled; }

	UFUNCTION(BlueprintPure, Category = "Acoustics|Debug")
	bool IsDebugRaysEnabled() const { return m_bDebugRaysEnabled; }

	UFUNCTION(BlueprintPure, Category = "Acoustics|Debug")
	bool IsDebugHitsEnabled() const { return m_bDebugHitsEnabled; }

	UFUNCTION(BlueprintPure, Category = "Acoustics|Debug", meta = (DisplayName = "Get Show Detector Positions"))
	bool IsDebugDetectorPositionsEnabled() const { return m_bDebugDetectorPositionsEnabled; }

	/** Toggles cross-room blending and forces immediate re-evaluation of all emitters. */
	UFUNCTION(BlueprintCallable, Category = "Acoustics|Features")
	void SetCrossRoomBlendingEnabled(bool bEnabled);

	/** Toggles the distance-based direct-to-reverb ratio (late reverb send scaling) and forces immediate re-evaluation. */
	UFUNCTION(BlueprintCallable, Category = "Acoustics|Features", meta = (DisplayName = "Set Direct-To-Reverb Ratio Enabled"))
	void SetDistanceAttenuationEnabled(bool bEnabled);

	UFUNCTION(BlueprintPure, Category = "Acoustics|Features")
	bool IsCrossRoomBlendingEnabled() const { return m_bCrossRoomEnabled; }

	UFUNCTION(BlueprintPure, Category = "Acoustics|Features", meta = (DisplayName = "Get Direct-To-Reverb Ratio Enabled"))
	bool IsDistanceAttenuationEnabled() const { return m_bDistanceAttenuationEnabled; }

	/** Switch ray scanning method at runtime. Restart is debounced internally. */
	UFUNCTION(BlueprintCallable, Category = "Acoustics|Parameters")
	void SetRayScanningMethod(ERayScanningMethod NewMethod);

	UFUNCTION(BlueprintPure, Category = "Acoustics|Parameters")
	ERayScanningMethod GetRayScanningMethod() const { return m_currentRayScanningMethod; }

	UFUNCTION(BlueprintCallable, Category = "Acoustics|Debug", meta = (DisplayName = "Set Reverb Solo Wet Enabled"))
	void SetReverbSoloWetEnabled(bool bEnabled);

	UFUNCTION(BlueprintPure, Category = "Acoustics|Debug")
	bool IsReverbSoloWetEnabled() const { return m_bReverbSoloWetEnabled; }

	/**
	 * Spawns a one-shot sound at the given world location and applies acoustics to it until playback finishes.
	 *
	 * Source of acoustics is resolved in this order:
	 *   1. SourceDetector != nullptr         -> re-uses its cached sends/ER (cheapest, works for fire-and-forget
	 *                                           sounds belonging to an existing detector actor).
	 *   2. bCreateTempDetector == true       -> spawns a hidden temporary UAcousticsDetectorComponent at Location;
	 *                                           Immediate detection when bImmediateDetection is true (accurate, slightly heavier).
	 *   3. Otherwise                         -> falls back to the listener detector (cheap, approximate, ignores the
	 *                                           source's room; adequate only when the sound is near the listener).
	 *
	 * bApplyReverbSends / bApplyEarlyReflections let callers opt out of individual DSP paths.
	 * The returned UAudioComponent is the same one that UGameplayStatics::SpawnSoundAttached returns, with bAutoDestroy=true.
	 */
	UFUNCTION(BlueprintCallable, Category = "Acoustics|OneShot",
		meta = (WorldContext = "WorldContextObject", AdvancedDisplay = "5",
				AutoCreateRefTerm = "Rotation"))
	UAudioComponent* PlaySoundAtLocationWithAcoustics(
		UObject* WorldContextObject,
		USoundBase* Sound,
		FVector Location,
		FRotator Rotation,
		UAcousticsDetectorComponent* SourceDetector = nullptr,
		bool bCreateTempDetector = false,
		bool bImmediateDetection = false,
		bool bApplyReverbSends = true,
		bool bApplyEarlyReflections = true,
		float VolumeMultiplier = 1.0f,
		float PitchMultiplier = 1.0f,
		float StartTime = 0.0f,
		USoundAttenuation* AttenuationSettings = nullptr,
		USoundConcurrency* ConcurrencySettings = nullptr,
		bool bAutoDestroy = true);

	/**
	 * Attaches acoustics to an existing UAudioComponent (e.g. one that Blueprint created via SpawnSoundAtLocation).
	 * Arguments mirror PlaySoundAtLocationWithAcoustics minus the spawn parameters.
	 * If AudioComponent has no owning actor, a hidden carrier actor is spawned at its world location so the adapter
	 * has a stable owner for submix lifecycle; the carrier is destroyed automatically with the binding.
	 */
	UFUNCTION(BlueprintCallable, Category = "Acoustics|OneShot")
	void AssignAcousticsToAudioComponent(
		UAudioComponent* AudioComponent,
		UAcousticsDetectorComponent* SourceDetector = nullptr,
		bool bCreateTempDetector = false,
		bool bImmediateDetection = false,
		bool bApplyReverbSends = true,
		bool bApplyEarlyReflections = true);

	/**
	 * Applies late reverb aux sends to an arbitrary UAudioComponent from processed send levels of
	 * a UAcousticsDetectorComponent, and optionally updates early reflections on that audio. The audio
	 * component's owner must be a valid actor. Call each frame while the sound should follow the analysis.
	 */
	UFUNCTION(BlueprintCallable, Category = "Acoustics",
		meta = (DisplayName = "Apply Acoustics To Audio Component", Keywords = "late reverb early reflections"))
	void ApplyAcousticsToAudioComponent(
		UAudioComponent* Audio,
		UAcousticsDetectorComponent* ReverbDataSource,
		bool bIgnoreCrossRoom = false,
		bool bIgnoreDirectToReverbRatio = false,
		UPARAM(DisplayName = "Enable ERs") bool bEnableERs = true,
		UPARAM(DisplayName = "ER Distance Scale") float EarlyReflectionsDistanceScale = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "Acoustics|Late Reverb",
		meta = (DisplayName = "Clear Late Reverb Sends On Audio Component"))
	void ClearLateReverbSendsOnAudioComponent(UAudioComponent* Audio);

private:
	void LoadGlobalSettings();
	void OnRaycastComplete(const FTraceHandle& TraceHandle, FTraceDatum& TraceData);
	EWAcoustics::SDK::ProbeResult CreateProbeResult(
		EWAcoustics::SDK::DetectorId DetectorId,
		const EWAcoustics::SDK::ProbeRequest& Request,
		const FHitResult& HitResult,
		const FVector& EndPoint
	);
	void ScheduleRaycastsForDetector(EWAcoustics::SDK::DetectorId DetectorId, AActor* OptionalIgnoreActor);
	/** Runs all rays synchronously in one frame for bImmediateDetection components. */
	void RunImmediateDetection(EWAcoustics::SDK::DetectorId DetectorId, UAcousticsDetectorComponent* Component);

	// Per-frame pipeline (called exactly once per world tick from Tick()).
	void TickListener();
	void TickAllDetectors(float DeltaTime);
	void SyncListenerDistanceToSdk(EWAcoustics::SDK::DetectorId DetectorId, const FVector& EmitterWorldPos);
	void ProcessDetectorStateChange(EWAcoustics::SDK::DetectorId DetectorId);

	void UpdatePerFrameStats();

#if !UE_BUILD_SHIPPING
	void DrawDebugDetectorPositions();
	void DrawDebugForVisibleDetectors();
	void DrawExternalDetectorDebug(EWAcoustics::SDK::DetectorId DetectorId, const FVector& WorldLocation);
#endif

private:
	using FDetectorManagerPtr = std::unique_ptr<EWAcoustics::SDK::AcousticsDetectorManager, EWAcoustics::SDK::AcousticsDetectorManagerDeleter>;
	FDetectorManagerPtr m_detectorManager;
	EWAcoustics::SDK::AcousticsConfig m_globalConfig;
	FVector m_listenerPosition;
	FVector m_listenerForward;
	FVector m_listenerUp;
	EWAcoustics::SDK::DetectorId m_listenerDetectorId;
	TMap<EWAcoustics::SDK::DetectorId, FAcousticsDetectorRecord> m_detectorRecords;

	struct FPendingRaycast
	{
		EWAcoustics::SDK::ProbeRequest Request;
		FVector EndPoint;
		EWAcoustics::SDK::DetectorId DetectorId;
		uint32_t DetectionGeneration;

		FPendingRaycast()
			: EndPoint(FVector::ZeroVector)
			, DetectorId(0)
			, DetectionGeneration(0)
		{
		}

		FPendingRaycast(
			EWAcoustics::SDK::DetectorId InDetectorId,
			const EWAcoustics::SDK::ProbeRequest& InRequest,
			const FVector& InEndPoint,
			uint32_t InGeneration
		)
			: Request(InRequest)
			, EndPoint(InEndPoint)
			, DetectorId(InDetectorId)
			, DetectionGeneration(InGeneration)
		{
		}
	};

	TMap<FTraceHandle, FPendingRaycast> m_pendingRaycasts;
	TMap<EWAcoustics::SDK::DetectorId, TSet<FTraceHandle>> m_pendingRaycastsByDetector;

	TMap<EWAcoustics::SDK::DetectorId, EWAcoustics::SDK::EDetectorState> m_detectorStates;
	TMap<EWAcoustics::SDK::DetectorId, FAcousticsDebugOverlayEntry> m_debugOverlayTexts;

	bool m_bSystemEnabled = true;
	bool m_bDebugTextEnabled = true;
	bool m_bDebugRaysEnabled = true;
	bool m_bDebugHitsEnabled = true;
	bool m_bDebugDetectorPositionsEnabled = false;
	bool m_bCrossRoomEnabled = true;
	bool m_bDistanceAttenuationEnabled = true;
	ERayScanningMethod m_currentRayScanningMethod = ERayScanningMethod::Umbrella;
	FTimerHandle m_parameterDebounceTimer;
	uint64 m_lastDetectorDrawFrame = 0;
	uint64 m_lastStatsUpdateFrame = 0;

	// Per-frame global raycast schedule budget counter. Paired with the SDK's
	// rate-limiting path (Settings->EnableRateLimiting + MaxRaysPerFrame); this
	// field only tracks the running count, the cap itself lives in SDK config.
	uint32 m_globalRaysScheduledThisFrame = 0;
	uint64 m_lastRaycastBudgetFrame = 0;

	int32 m_maxActiveDetectors = 0;
	int32 m_detectorTickFrameSpread = 1;

	bool m_bReverbSoloWetEnabled = false;
	TWeakObjectPtr<USoundSubmix> m_wetSoloDrySubmix;
	float m_wetSoloSavedDryLinear = 1.0f;

	void ForceReEvaluateAllEmitters();
	void PropagateConfigAndRestart(bool bRestart);
	void ClearAllEmitterSends();
	void PruneInvalidExternalReverbAdapters();
	void ShutdownAllExternalReverbAdapters();
	void ClearPendingRaycastsForDetector(EWAcoustics::SDK::DetectorId DetectorId);
	void ScheduleDebouncedRestart();
	void TickExternalReverbAdapters();

	struct FExternalReverbEntry
	{
		TSharedPtr<IAcousticsAudioAdapter> Adapter;
		EWAcoustics::SDK::DetectorId SourceDetectorId = 0;
		bool bEnableERs = true;
		float EarlyReflectionsDistanceScale = 1.0f;
	};

	TMap<TWeakObjectPtr<UAudioComponent>, FExternalReverbEntry> m_externalReverbByAudio;

	// ---- One-shot bindings (PlaySoundAtLocationWithAcoustics / AssignAcousticsToAudioComponent) ----

	struct FOneShotBinding
	{
		TWeakObjectPtr<UAudioComponent> AudioComponent;
		TWeakObjectPtr<AAcousticsOneShotCarrier> AudioCarrier;
		TWeakObjectPtr<AAcousticsOneShotCarrier> DetectorCarrier;
		TSharedPtr<IAcousticsAudioAdapter> Adapter;
		EWAcoustics::SDK::DetectorId SourceDetectorId = 0;  // 0 == listener fallback
		bool bOwnsTempDetector = false;
		bool bApplyReverbSends = true;
		bool bApplyEarlyReflections = true;
	};

	TArray<FOneShotBinding> m_oneShotBindings;
	FTSTicker::FDelegateHandle m_oneShotTickerHandle;

	bool TickOneShots(float DeltaTime);
	void TickOneShotBinding(FOneShotBinding& Binding);
	void ShutdownOneShotBinding(FOneShotBinding& Binding);
	bool FinalizeOneShotBinding(
		UAudioComponent* AudioComponent,
		AAcousticsOneShotCarrier* AudioCarrier,
		UAcousticsDetectorComponent* SourceDetector,
		bool bCreateTempDetector,
		bool bImmediateDetection,
		bool bApplyReverbSends,
		bool bApplyEarlyReflections);
};

