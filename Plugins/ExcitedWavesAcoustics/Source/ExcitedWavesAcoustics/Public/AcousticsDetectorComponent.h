// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ExcitedWavesAcousticsSettings.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "Templates/UnrealTemplate.h"
#include "IAcousticsAudioAdapter.h"
#include "AcousticsBPProcessedSends.h"
#include "AcousticsDetectorComponent.generated.h"

class UAcousticsSubsystem;

USTRUCT(BlueprintType)
struct FAcousticsDebugOverlayData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Acoustics") FString ActorName;

	UPROPERTY(BlueprintReadOnly, Category = "Acoustics") float Close = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Acoustics") float Mid = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Acoustics") float Far = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Acoustics") float Outdoor = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Acoustics") float OriginalClose = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Acoustics") float OriginalMid = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Acoustics") float OriginalFar = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Acoustics") float OriginalOutdoor = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Acoustics") float Absorption = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Acoustics") float ReverbEnclosure = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Acoustics") float Height = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Acoustics") float Width = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Acoustics") float DistanceToListener = 0.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Acoustics") float ReverbMultiplier = 1.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Acoustics") float CrossRoomContribution = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Acoustics") bool bHasData = false;
	UPROPERTY(BlueprintReadOnly, Category = "Acoustics") bool bIsIndoor = false;
};

USTRUCT()
struct FDebugRayData
{
	GENERATED_BODY()

	FVector Origin;
	FVector Direction;
	FVector HitPosition;
	float MaxDistance;
	float Timestamp;
	EWAcoustics::SDK::ERayType RayType;
	bool bHit;
	bool bIsVerificationRay;

	FDebugRayData()
		: Origin(FVector::ZeroVector)
		, Direction(FVector::ZeroVector)
		, HitPosition(FVector::ZeroVector)
		, MaxDistance(0.0f)
		, Timestamp(0.0f)
		, RayType(EWAcoustics::SDK::ERayType::Side)
		, bHit(false)
		, bIsVerificationRay(false)
	{
	}
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EXCITEDWAVESACOUSTICS_API UAcousticsDetectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UAcousticsDetectorComponent();
	virtual ~UAcousticsDetectorComponent();

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	// UActorComponent::TickComponent is intentionally not overridden and tick is
	// disabled in the constructor. The subsystem drives this component via
	// DriveFromSubsystem() once per frame for every registered component detector.
	void DriveFromSubsystem(UAcousticsSubsystem* Subsystem, float DeltaTime, bool bSubsystemTickedThisDetector = true);

#if !UE_BUILD_SHIPPING
	void Subsystem_DrawDebug(bool bDrawRays, bool bDrawHits, bool bDrawText);
#endif

	UFUNCTION(BlueprintCallable, Category = "Excited Waves Acoustics|Analysis", CallInEditor)
	float GetCloseReverbPercentage() const;

	UFUNCTION(BlueprintCallable, Category = "Excited Waves Acoustics|Analysis", CallInEditor)
	float GetMidReverbPercentage() const;

	UFUNCTION(BlueprintCallable, Category = "Excited Waves Acoustics|Analysis", CallInEditor)
	float GetFarReverbPercentage() const;

	UFUNCTION(BlueprintCallable, Category = "Excited Waves Acoustics|Analysis", CallInEditor)
	float GetOutdoorReverbPercentage() const;

	/** True once the first detection cycle has completed and results are valid. */
	UFUNCTION(BlueprintCallable, Category = "Excited Waves Acoustics|Analysis", CallInEditor)
	bool IsAnalysisReady() const;

	/** Surface absorption coefficient derived from ray hits (0 = reflective, 1 = absorbing). */
	UFUNCTION(BlueprintCallable, Category = "Excited Waves Acoustics|Analysis", CallInEditor)
	float GetAbsorption() const;

	UFUNCTION(BlueprintCallable, Category = "Excited Waves Acoustics|Analysis", CallInEditor)
	bool GetIsIndoor() const;

	UFUNCTION(BlueprintCallable, Category = "Excited Waves Acoustics|Debug UI")
	FAcousticsDebugOverlayData GetDebugOverlayData() const;

	UFUNCTION(BlueprintPure, Category = "Excited Waves Acoustics|Analysis",
		meta = (DisplayName = "Get Detector Processed Sends", CompactNodeTitle = "Get Proc. Sends", Keywords = "reverb send processed GetDetectorProcessedSends"))
	FAcousticsBPProcessedSendLevels GetDetectorProcessedSends() const;

	UFUNCTION(BlueprintCallable, Category = "Excited Waves Acoustics|Detection")
	void SetDetectorEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Excited Waves Acoustics|Detection")
	bool IsDetectorEnabled() const { return bDetectorEnabled; }

	/** Restart detection from scratch. Use after geometry changes (destruction, etc.). */
	UFUNCTION(BlueprintCallable, Category = "Excited Waves Acoustics|Detection")
	void ForceRecalculation();

	UFUNCTION(BlueprintCallable, Category = "Excited Waves Acoustics|Early Reflections",
		meta = (DisplayName = "Set Early Reflections Enabled",
			ToolTip = "Turns early reflections on or off for this detector and immediately syncs the audio adapter."))
	void SetEarlyReflectionsEnabled(bool bEnabled);

	void ClearAuxSends();
	void UpdateAcousticsSends();
	bool ShouldRunHeavyDetectorTickForAudio() const;
	EWAcoustics::SDK::DetectorId GetDetectorId() const { return m_detectorId; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Excited Waves Acoustics|Detection", meta = (DisplayName = "Enabled", ToolTip = "When disabled, this detector does not register, schedule raycasts, or run analysis. Toggle at runtime to activate/deactivate."))
	bool bDetectorEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Excited Waves Acoustics|Detection", meta = (DisplayName = "Immediate First Detection", ToolTip = "Complete the first detection cycle synchronously so reverb is ready immediately. Good for runtime-spawned short sounds. Costs more in the spawn frame."))
	bool bImmediateDetection = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Excited Waves Acoustics|Detection", meta = (DisplayName = "Is Listener", ToolTip = "When enabled, this detector is registered as the listener. Its reverb analysis is used for cross-room reverb blending with emitters."))
	bool bIsListener = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Excited Waves Acoustics|Late Reverbs", meta = (DisplayName = "Ignore Cross-Room For This Detector", ToolTip = "When enabled, this detector ignores cross-room late reverb contribution even if the feature is enabled globally. Useful for actors that should stay local (e.g. player-controlled emitters)."))
	bool bIgnoreCrossRoomForThisDetector = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Excited Waves Acoustics|Late Reverbs", meta = (DisplayName = "Ignore Direct-To-Reverb Ratio For This Detector", ToolTip = "When enabled, this detector ignores listener-distance direct-to-reverb ratio scaling for late reverb sends, even if enabled globally."))
	bool bIgnoreDirectToReverbRatioForThisDetector = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Excited Waves Acoustics|Early Reflections", meta = (DisplayName = "Enable Reflections", ToolTip = "Enable early reflections for this emitter. Disable for listener detectors."))
	bool bEnableReflections = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Excited Waves Acoustics|Early Reflections", meta = (DisplayName = "Distance Scaling Factor", ClampMin = "0.1", ClampMax = "10.0", ToolTip = "Wwise Reflect distance scaling on image sources. 1.0 matches physical mirror positions from ray hits; values above 1 push reflections farther in the plugin."))
	float ReflectDistanceScalingFactor = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Excited Waves Acoustics|Early Reflections", meta = (DisplayName = "Reflect Level", ClampMin = "0.0", ClampMax = "1.0", ToolTip = "Reflections volume level for this emitter (0 = mute, 1 = full)"))
	float ReflectLevel = 1.0f;

private:

	const UExcitedWavesAcousticsSettings* GetEnvironmentalSettings() const;
	bool RegisterWithSubsystem();
	void CheckDetectorStateChange(UAcousticsSubsystem* Subsystem);
#if !UE_BUILD_SHIPPING
	void DrawDebugInfo();
	void DrawDebugTextAboveActor();
	FVector GetViewerPosition() const;
	bool ShouldDrawDebugAtPosition(const FVector& Position) const;
	FString CreateDebugText() const;
	static FString GetStateString(EWAcoustics::SDK::EDetectorState State);
#endif
	const EWAcoustics::SDK::AcousticsResults* GetResults() const;
	void SyncReflectionSettingsToAdapter();

	TSharedPtr<IAcousticsAudioAdapter> m_audioAdapter;
	EWAcoustics::SDK::DetectorId m_detectorId;
#if !UE_BUILD_SHIPPING
	float m_lastSendClose;
	float m_lastSendMid;
	float m_lastSendFar;
	float m_lastSendOutdoor;
	float m_lastDistanceToListener;
	float m_lastReverbMultiplier;
	float m_lastCrossRoomContrib;
#endif
	float m_lastReflectUpdateTime = -1.0f;

	FAcousticsReverbSendData m_cachedSendData;
	bool m_bCachedSendDataValid = false;
	bool m_bWasAudioSourceActive = false;

	EWAcoustics::SDK::EDetectorState m_previousState;
	bool m_bAnalysisCompletedOnce;
	bool m_bReflectDistanceCulled = false;
	bool m_bSendsCulledByBudget = false;
	bool m_bReflectCulledByBudget = false;

	UPROPERTY(EditAnywhere, Category = "Excited Waves Acoustics|Audio", meta = (ToolTip = "Automatically update reverb aux sends based on analysis results. When enabled, UpdateAcousticsSends() is called automatically - no manual Blueprint calls needed."))
	bool bAutoUpdateAcoustics = true;

	UPROPERTY(EditAnywhere, Category = "Excited Waves Acoustics|Debug|Reflections", meta = (DisplayName = "Reflect Debug Color", EditCondition = "bShowReflectDebug", ToolTip = "Color for this emitter's reflection debug spheres"))
	FColor ReflectDebugColor = FColor(0, 240, 255);

	UPROPERTY(EditAnywhere, Category = "Excited Waves Acoustics|Debug|Text", meta = (ToolTip = "Height above the component for debug text (cm)"))
	float DebugTextDistance = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Excited Waves Acoustics|Debug|Text", meta = (ToolTip = "Horizontal offset to the right of the component for debug text (cm)"))
	float DebugTextOffsetRight = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Excited Waves Acoustics|Debug|Text", meta = (ToolTip = "Additional height offset above the component for debug text (cm)"))
	float DebugTextOffsetUp = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Excited Waves Acoustics|Debug|Text", meta = (ToolTip = "Shift the whole debug overlay up (positive) or down (negative) in cm"))
	float DebugTextVerticalOffset = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Excited Waves Acoustics|Debug|Text", meta = (ToolTip = "Scale factor for debug text size"))
	float DebugTextScale = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Excited Waves Acoustics|Debug", meta = (ToolTip = "When enabled, this component is never drawn in debug visualization (rays, hit points, text, reflection spheres), regardless of distance"))
	bool bIgnoreInDebugDraw = false;

	UPROPERTY(EditAnywhere, Category = "Excited Waves Acoustics|Debug|Reflections", meta = (DisplayName = "Show Reflect Debug", ToolTip = "Draw debug spheres at this emitter's image source positions. Global 'Show Reflect Hit Points' must also be enabled."))
	bool bShowReflectDebug = false;

	UPROPERTY(EditAnywhere, Category = "Excited Waves Acoustics|Debug|Text", meta = (ToolTip = "Enable debug text display (also requires 'Show Results' in Project Settings)"))
	bool bShowDebugText = true;

	UPROPERTY(EditAnywhere, Category = "Excited Waves Acoustics|Debug|Text", meta = (ToolTip = "Draw debug text in UI overlay (read via Acoustics Subsystem Get Debug Overlay Texts) instead of in-world"))
	bool bDrawDebugTextOnScreen = false;
};

