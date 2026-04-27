// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ExcitedWavesAcousticsSDK/Public/AcousticsTypes.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Curves/CurveFloat.h"
#include "ExcitedWavesAcousticsSettings.generated.h"

struct FHitResult;
struct FAcousticsReverbSendData;

UENUM(BlueprintType)
enum class ERayScanningMethod : uint8
{
	Umbrella	UMETA(DisplayName = "Umbrella", ToolTip = "Fixed horizontal pairs with elevation iterations"),
	Uniform	UMETA(DisplayName = "Uniform", ToolTip = "Uniform: uniform distribution, classified as up/down/side")
};

UENUM(BlueprintType)
enum class EDebugDisplayMode : uint8
{
	Full	UMETA(DisplayName = "Full", ToolTip = "Full debug overlay: state, heights, counters, distance bins, frame limiting, etc."),
	Brief	UMETA(DisplayName = "Brief", ToolTip = "Brief overlay: sum of heights, width, close/mid/far/outdoor, enclosure, indoor, mean absorption")
};

UENUM(BlueprintType)
enum class ECurveShape : uint8
{
	Linear		UMETA(DisplayName = "Linear", ToolTip = "Linear ramp from 0 to 1 over distance"),
	Smooth		UMETA(DisplayName = "Smooth", ToolTip = "Smoothstep: gradual ease-in-out"),
	Exponential	UMETA(DisplayName = "Exponential", ToolTip = "1 - exp(-k*dist): fast rise, natural for reverb"),
	Inverse		UMETA(DisplayName = "Inverse", ToolTip = "Fast at start, slows toward 1"),
	Curve		UMETA(DisplayName = "Curve", ToolTip = "Use CurveFloat asset — select curve below")
};

USTRUCT()
struct EXCITEDWAVESACOUSTICS_API FSurfaceTypeAbsorptionEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Extra", meta = (ToolTip = "Physics Surface Type from Physical Material. Configure in Project Settings > Physics > Physical Surface."))
	TEnumAsByte<EPhysicalSurface> SurfaceType = SurfaceType_Default;

	UPROPERTY(EditAnywhere, Category = "Extra", meta = (ClampMin = "0.0", ClampMax = "1.0", ToolTip = "Absorption coefficient for this surface type. 0.0 = fully reflective, 1.0 = fully absorbing"))
	float AbsorptionCoefficient = 0.0f;


	FSurfaceTypeAbsorptionEntry() = default;
	FSurfaceTypeAbsorptionEntry(EPhysicalSurface InSurfaceType, float InAbsorption)
		: SurfaceType(InSurfaceType)
		, AbsorptionCoefficient(InAbsorption)
	{
	}
};

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Excited Waves Acoustics"))
class EXCITEDWAVESACOUSTICS_API UExcitedWavesAcousticsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UExcitedWavesAcousticsSettings();

	virtual FName GetCategoryName() const override;
#if WITH_EDITOR
	virtual FText GetSectionDescription() const override;
#endif

	UPROPERTY(EditAnywhere, Config, Category = "LOD", meta = (DisplayName = "Position Change Threshold (cm)", ClampMin = "1.0", ClampMax = "1000.0", EditCondition = "!EnableDistanceBasedLOD", ToolTip = "Default threshold when Distance LOD is disabled. Ignored when LOD is enabled."))
	float PositionChangeThreshold = 100.0f;

	UPROPERTY(EditAnywhere, Config, Category = "LOD", meta = (DisplayName = "Enable Distance-Based LOD", ToolTip = "When enabled, SDK uses close vs far position-change thresholds to decide when to restart analysis after movement, based on distance from emitter to listener."))
	bool EnableDistanceBasedLOD = true;

	UPROPERTY(EditAnywhere, Config, Category = "LOD", meta = (DisplayName = "LOD Distance Radius (cm)", ClampMin = "100.0", ClampMax = "10000.0", EditCondition = "EnableDistanceBasedLOD", ToolTip = "Distance from listener: inside this radius uses Close Emitter Threshold for restart-on-move; outside uses Far Emitter Threshold. Unrelated to how many detectors are selected for analysis."))
	float LODDistanceRadius = 6000.0f;

	UPROPERTY(EditAnywhere, Config, Category = "LOD", meta = (DisplayName = "Close Emitter Threshold (cm)", ClampMin = "1.0", ClampMax = "1000.0", EditCondition = "EnableDistanceBasedLOD", ToolTip = "Minimum movement (cm) to restart analysis when emitter is inside LOD Distance Radius."))
	float CloseEmitterThreshold = 100.0f;

	UPROPERTY(EditAnywhere, Config, Category = "LOD", meta = (DisplayName = "Far Emitter Threshold (cm)", ClampMin = "1.0", ClampMax = "2000.0", EditCondition = "EnableDistanceBasedLOD", ToolTip = "Minimum movement (cm) to restart analysis when emitter is outside LOD Distance Radius."))
	float FarEmitterThreshold = 400.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Debug|General", meta = (DisplayName = "Debug Display Mode", ToolTip = "Full: detailed overlay. Brief: compact summary."))
	EDebugDisplayMode DebugDisplayMode = EDebugDisplayMode::Brief;

	UPROPERTY(EditAnywhere, Config, Category = "Debug|General", meta = (DisplayName = "Show Debug Rays", ToolTip = "Draw ray lines from detector to hit points"))
	bool ShowDebugRays = false;

	UPROPERTY(EditAnywhere, Config, Category = "Debug|General", meta = (DisplayName = "Show Hit Points", ToolTip = "Draw spheres at ray hit points"))
	bool ShowHitPoints = false;

	UPROPERTY(EditAnywhere, Config, Category = "Debug|General", meta = (DisplayName = "Show Results", ToolTip = "Show on-screen overlay with acoustics metrics"))
	bool ShowResults = false;

	UPROPERTY(EditAnywhere, Config, Category = "Debug|General", meta = (DisplayName = "Show Detector Positions", ToolTip = "Draw debug points at each detector's position. Colored active, gray inactive."))
	bool ShowDetectorPositions = false;

	UPROPERTY(EditAnywhere, Config, Category = "Debug|Visualization", meta = (DisplayName = "Debug Draw Min Distance (cm)", ClampMin = "0.0", ToolTip = "Minimum distance from camera/player to draw debug visualization; components closer than this are not drawn (0 = no minimum)"))
	float DebugDrawMinDistance = 50.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Debug|Visualization", meta = (DisplayName = "Debug Draw Max Distance (cm)", ClampMin = "0.0", ToolTip = "Maximum distance from camera/player to draw debug visualization (0 = unlimited, recommended: 5000-10000)"))
	float DebugDrawDistance = 1000.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Debug", meta = (DisplayName = "Debug Ray Lifetime (seconds)", ClampMin = "0.1", ClampMax = "60.0", ToolTip = "How long debug rays stay visible after a rescan. When a detector re-scans (emitter moved far enough), rays are refreshed and re-drawn for this duration."))
	float DebugRayLifetime = 1.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Debug", meta = (DisplayName = "Debug Line Thickness", ClampMin = "1.0", ClampMax = "10.0", ToolTip = "Line thickness for debug rays and hit point crosses"))
	float DebugLineThickness = 1.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Debug", meta = (DisplayName = "Hit Point Size", ClampMin = "1.0", ClampMax = "50.0", ToolTip = "Size of debug points drawn at ray hit points"))
	float DebugHitPointSize = 5.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Debug", meta = (DisplayName = "Detector Point Base Size", ClampMin = "1.0", ClampMax = "50.0", EditCondition = "ShowDetectorPositions", ToolTip = "Size of debug points drawn at detector positions."))
	float DetectorPointBaseSize = 10.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Debug", meta = (DisplayName = "Side", ToolTip = "Color for hit points from horizontal (side) rays"))
	FColor DebugHitPointColorSide = FColor(255, 230, 0);

	UPROPERTY(EditAnywhere, Config, Category = "Debug", meta = (DisplayName = "Up", ToolTip = "Color for hit points from upward rays"))
	FColor DebugHitPointColorUp = FColor(0, 240, 255);

	UPROPERTY(EditAnywhere, Config, Category = "Debug", meta = (DisplayName = "Down", ToolTip = "Color for hit points from downward rays"))
	FColor DebugHitPointColorDown = FColor(0, 240, 255);

	UPROPERTY(EditAnywhere, Config, Category = "Debug", meta = (DisplayName = "Verification", ToolTip = "Color for hit points that require verification"))
	FColor DebugHitPointColorVerification = FColor(34, 197, 94);

	UPROPERTY(EditAnywhere, Config, Category = "Debug", meta = (DisplayName = "Side Ray Color", ToolTip = "Color for regular horizontal (side) rays"))
	FColor DebugRayColorSide = FColor(255, 230, 0);

	UPROPERTY(EditAnywhere, Config, Category = "Debug", meta = (DisplayName = "Up Ray Color", ToolTip = "Color for upward rays that hit"))
	FColor DebugRayColorUp = FColor(0, 240, 255);

	UPROPERTY(EditAnywhere, Config, Category = "Debug", meta = (DisplayName = "Down Ray Color", ToolTip = "Color for downward rays that hit"))
	FColor DebugRayColorDown = FColor(0, 240, 255);

	UPROPERTY(EditAnywhere, Config, Category = "Debug", meta = (DisplayName = "Verification Ray Color", ToolTip = "Color for verification rays that hit"))
	FColor DebugRayColorVerification = FColor(34, 197, 94);

	UPROPERTY(EditAnywhere, Config, Category = "Debug", meta = (DisplayName = "Unverified Ray Color", ToolTip = "Color for side rays that failed verification"))
	FColor DebugRayColorUnverified = FColor(170, 136, 0);

	UPROPERTY(EditAnywhere, Config, Category = "Debug", meta = (DisplayName = "Missed Ray Color", ToolTip = "Color for rays that did not hit anything"))
	FColor DebugRayColorMissed = FColor(85, 85, 85);

	UPROPERTY(EditAnywhere, Config, Category = "Debug", meta = (DisplayName = "Outlier Ray Color", ToolTip = "Color for rays detected as outliers"))
	FColor DebugRayColorOutlier = FColor(232, 48, 48);

	UPROPERTY(EditAnywhere, Config, Category = "Extra", meta = (DisplayName = "Enable Cross-Room Reverb Blending", ToolTip = "When enabled, a portion of the listener's room reverb is added to each emitter's sound based on distance. This simulates sound picking up reverb characteristics of the listener's room as it travels between rooms."))
	bool bEnableCrossRoomReverbBlending = true;

	UPROPERTY(EditAnywhere, Config, Category = "Extra", meta = (DisplayName = "Cross-Room Blend Curve Shape", EditCondition = "bEnableCrossRoomReverbBlending", ToolTip = "Linear/Smooth/Exponential/Inverse: programmatic curve. Curve: use CurveFloat asset below."))
	ECurveShape CrossRoomBlendCurveShape = ECurveShape::Exponential;

	UPROPERTY(EditAnywhere, Config, Category = "Extra", meta = (DisplayName = "Cross-Room Blend Min Distance (cm)", ClampMin = "0.0", ClampMax = "10000.0", EditCondition = "bEnableCrossRoomReverbBlending && CrossRoomBlendCurveShape != ECurveShape::Curve", EditConditionHides, ToolTip = "Below this distance, output is constant (Min Value). 0 = no flat zone."))
	float CrossRoomBlendMinDistance = 0.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Extra", meta = (DisplayName = "Cross-Room Blend Max Distance (cm)", ClampMin = "100.0", ClampMax = "50000.0", EditCondition = "bEnableCrossRoomReverbBlending && CrossRoomBlendCurveShape != ECurveShape::Curve", EditConditionHides, ToolTip = "Distance at which blend reaches full. Used when Curve Shape is Linear/Smooth/Exponential/Inverse."))
	float CrossRoomBlendMaxDistance = 1000.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Extra", meta = (DisplayName = "Cross-Room Blend Min Value", ClampMin = "0.0", ClampMax = "1.0", EditCondition = "bEnableCrossRoomReverbBlending && CrossRoomBlendCurveShape != ECurveShape::Curve", EditConditionHides, ToolTip = "Constant output when distance is below Min Distance. 0 = no cross-room when close."))
	float CrossRoomBlendMinValue = 0.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Extra", meta = (DisplayName = "Cross-Room Blend Curve", EditCondition = "bEnableCrossRoomReverbBlending && CrossRoomBlendCurveShape == ECurveShape::Curve", EditConditionHides, ToolTip = "CurveFloat asset: X = distance (cm), Y = blend factor. Shown when Curve Shape = Curve."))
	TSoftObjectPtr<UCurveFloat> CrossRoomBlendCurveOverride;

	UPROPERTY(EditAnywhere, Config, Category = "Extra", meta = (DisplayName = "Max Cross-Room Contribution", ClampMin = "0.0", ClampMax = "1.0", EditCondition = "bEnableCrossRoomReverbBlending", ToolTip = "Maximum amount of the listener's reverb that can be added (0–1). Scales the final blended listener reverb contribution."))
	float MaxCrossRoomReverbContribution = 0.5f;

	UPROPERTY(EditAnywhere, Config, Category = "Extra", meta = (DisplayName = "Enable Direct-To-Reverb Ratio", ToolTip = "When enabled, late reverb send levels are scaled by distance from emitter to listener (direct-to-reverb balance: closer typically less late reverb, farther toward full evaluated wet). Does not change Unreal dry attenuation. Requires a Listener detector."))
	bool bEnableDistanceReverbAttenuation = true;

	UPROPERTY(EditAnywhere, Config, Category = "Extra", meta = (DisplayName = "Direct-To-Reverb Ratio Curve Shape", EditCondition = "bEnableDistanceReverbAttenuation", ToolTip = "How the late-reverb scale (0–1) changes with distance to the listener. Linear/Smooth/Exponential/Inverse: built-in. Curve: use CurveFloat asset below."))
	ECurveShape DistanceAttenuationCurveShape = ECurveShape::Exponential;

	UPROPERTY(EditAnywhere, Config, Category = "Extra", meta = (DisplayName = "Direct-To-Reverb Ratio Min Distance (cm)", ClampMin = "0.0", ClampMax = "10000.0", EditCondition = "bEnableDistanceReverbAttenuation && DistanceAttenuationCurveShape != ECurveShape::Curve", EditConditionHides, ToolTip = "Below this distance to the listener, the late-reverb scale stays at Min Value. 0 = no flat zone."))
	float DistanceAttenuationMinDistance = 0.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Extra", meta = (DisplayName = "Direct-To-Reverb Ratio Max Distance (cm)", ClampMin = "100.0", ClampMax = "50000.0", EditCondition = "bEnableDistanceReverbAttenuation && DistanceAttenuationCurveShape != ECurveShape::Curve", EditConditionHides, ToolTip = "Distance to the listener at which the late-reverb scale reaches 1. Used when Curve Shape is Linear/Smooth/Exponential/Inverse."))
	float DistanceAttenuationMaxDistance = 2000.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Extra", meta = (DisplayName = "Direct-To-Reverb Ratio Min Value", ClampMin = "0.0", ClampMax = "1.0", EditCondition = "bEnableDistanceReverbAttenuation && DistanceAttenuationCurveShape != ECurveShape::Curve", EditConditionHides, ToolTip = "Late-reverb scale when distance is below Min Distance. 0.3–0.5 avoids fully dry late reverb when very close."))
	float DistanceAttenuationMinValue = 0.5f;

	UPROPERTY(EditAnywhere, Config, Category = "Extra", meta = (DisplayName = "Direct-To-Reverb Ratio Curve", EditCondition = "bEnableDistanceReverbAttenuation && DistanceAttenuationCurveShape == ECurveShape::Curve", EditConditionHides, ToolTip = "CurveFloat: X = distance to listener (cm), Y = late-reverb scale (0–1). Shown when Curve Shape = Curve."))
	TSoftObjectPtr<UCurveFloat> DistanceAttenuationCurveOverride;

	UPROPERTY(EditAnywhere, Config, Category = "Extra", meta = (DisplayName = "Enable Enclosure Scaling", ToolTip = "Scale reverb sends by how enclosed the space is. Open spaces (high canopy, outdoor overhang) get less reverb; enclosed rooms get full reverb."))
	bool bEnableEnclosureScaling = true;

	UPROPERTY(EditAnywhere, Config, Category = "Extra", meta = (DisplayName = "Enclosure Scale Min", ClampMin = "0.0", ClampMax = "1.0", EditCondition = "bEnableEnclosureScaling", ToolTip = "Minimum reverb multiplier in fully open space (enclosure=0). At enclosure=1 (fully enclosed), multiplier is 1.0. Default 0.3 means open spaces retain 30% of reverb."))
	float EnclosureScaleMin = 0.3f;

	UPROPERTY(EditAnywhere, Config, Category = "Detection", meta = (DisplayName = "Max Azimuth Ray Distance (cm)", ClampMin = "100.0", ClampMax = "10000.0", ToolTip = "Maximum ray distance in horizontal (azimuth) plane."))
	float AcousticsMaxAzimuthRayDistance = 10000.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Detection", meta = (DisplayName = "Max Elevation Ray Distance (cm)", ClampMin = "100.0", ClampMax = "5000.0", ToolTip = "Maximum ray distance in vertical (elevation) direction."))
	float AcousticsMaxElevationRayDistance = 5000.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Detection", meta = (DisplayName = "Max Probe Height (cm)", ClampMin = "10.0", ClampMax = "1000.0", ToolTip = "Upper limit (cm) for horizontal probe elevation spread in Umbrella mode. Probes will not be generated above this height."))
	float AcousticsMaxProbeHeight = 1000.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Detection", meta = (DisplayName = "Ceiling Elevation Rays Offset (cm)", ClampMin = "1.0", ClampMax = "100.0", ToolTip = "Vertical offset subtracted from dynamic ceiling height when calculating per-iteration elevation spread. Prevents horizontal probes from spawning too close to the detected ceiling."))
	float AcousticsCeilingElevationRaysOffset = 10.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Detection", meta = (DisplayName = "Emitter Elevation Rays Offset (cm)", ClampMin = "1.0", ClampMax = "100.0", ToolTip = "Vertical offset for emitter elevation rays. Affects ray origin height relative to emitter."))
	float AcousticsEmitterElevationRaysOffset = 10.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Detection", meta = (DisplayName = "Detector Circle Radius (cm)", ClampMin = "10.0", ClampMax = "500.0", ToolTip = "Radius of the circle around the detector used for ray origin offsets"))
	float AcousticsPlayerCircleRadius = 150.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Detection", meta = (DisplayName = "Ray Scanning Method", ToolTip = "Umbrella: fixed horizontal pairs with elevation iterations. Uniform: uniform sphere distribution."))
	ERayScanningMethod AcousticsRayScanningMethod = ERayScanningMethod::Umbrella;

	UPROPERTY(EditAnywhere, Config, Category = "Detection", meta = (DisplayName = "Pairs Per Iteration", ClampMin = "1", ClampMax = "10", EditCondition = "AcousticsRayScanningMethod==ERayScanningMethod::Umbrella", ToolTip = "Ray pairs emitted per azimuth iteration in Umbrella mode."))
	int32 AcousticsPairsPerStep = 2;

	UPROPERTY(EditAnywhere, Config, Category = "Detection", meta = (DisplayName = "Azimuth Iterations", ClampMin = "1", ClampMax = "20", EditCondition = "AcousticsRayScanningMethod==ERayScanningMethod::Umbrella", ToolTip = "Number of azimuth iterations in Umbrella mode. Each iteration rotates the probe pattern."))
	int32 AcousticsAzimuthSteps = 1;

	UPROPERTY(EditAnywhere, Config, Category = "Detection", meta = (DisplayName = "Elevation Rays", ClampMin = "1", ClampMax = "10", EditCondition = "AcousticsRayScanningMethod==ERayScanningMethod::Umbrella", ToolTip = "Elevation rays per azimuth direction in Umbrella method."))
	int32 AcousticsElevationRays = 1;

	UPROPERTY(EditAnywhere, Config, Category = "Detection", meta = (DisplayName = "Angled Ray Iterations", ClampMin = "0", ClampMax = "4", EditCondition = "AcousticsRayScanningMethod==ERayScanningMethod::Umbrella", ToolTip = "Number of additional ray iterations fired at upward angles. These rays detect ceilings and overhangs in semi-open spaces (pavilions, covered walkways). Each iteration fires Pairs Per Step pairs at an elevated angle. 0 = disabled, 2 = recommended (30 and 55 degrees)."))
	int32 AcousticsAngledRayIterations = 2;

	UPROPERTY(EditAnywhere, Config, Category = "Detection", meta = (DisplayName = "Angled Base Elevation (degrees)", ClampMin = "10.0", ClampMax = "80.0", EditCondition = "AcousticsRayScanningMethod==ERayScanningMethod::Umbrella && AcousticsAngledRayIterations>0", ToolTip = "Starting elevation angle for the first angled ray iteration. 30 degrees catches nearby roofs and overhangs."))
	float AcousticsAngledRayBaseElevation = 30.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Detection", meta = (DisplayName = "Angled Elevation Increment (degrees)", ClampMin = "5.0", ClampMax = "45.0", EditCondition = "AcousticsRayScanningMethod==ERayScanningMethod::Umbrella && AcousticsAngledRayIterations>1", ToolTip = "Elevation increment between angled ray iterations. With base=30 and increment=25, two iterations fire at 30 and 55 degrees."))
	float AcousticsAngledRayElevationStep = 25.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Detection", meta = (DisplayName = "Uniform Rays Count", ClampMin = "6", ClampMax = "100", EditCondition = "AcousticsRayScanningMethod==ERayScanningMethod::Uniform", ToolTip = "Total rays for Uniform method. Distributed uniformly, then classified as up/down/side."))
	int32 AcousticsUniformRaysCount = 16;

	UPROPERTY(EditAnywhere, Config, Category = "Detection", meta = (DisplayName = "Close Distance (cm)", ClampMin = "0.0", ClampMax = "100000.0", ToolTip = "Analysis bin: close distance boundary"))
	float AcousticsCloseDistance = 360.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Detection", meta = (DisplayName = "Mid Distance (cm)", ClampMin = "0.0", ClampMax = "100000.0", ToolTip = "Analysis bin: mid distance boundary"))
	float AcousticsMidDistance = 1000.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Detection", meta = (DisplayName = "Far Distance (cm)", ClampMin = "0.0", ClampMax = "100000.0", ToolTip = "Analysis bin: far distance boundary"))
	float AcousticsFarDistance = 5000.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Load Balancing", meta = (DisplayName = "Maximum Active Detectors", ClampMin = "0", ToolTip = "When greater than 0, limits heavy acoustics work per frame to the listener, in-flight analysis, and the nearest active emitters up to this count. 0 = unlimited."))
	int32 MaxActiveDetectors = 32;

	UPROPERTY(EditAnywhere, Config, Category = "Load Balancing", meta = (DisplayName = "Detector Tick Frame Spread", ClampMin = "1", ClampMax = "32", ToolTip = "Spread per-detector subsystem work across N engine frames. A detector gets Set, full SDK update, and Drive on frames where (detectorId mod N) == (GFrameNumber mod N). The listener and detectors with in-flight async line traces are exempt and run every frame. Component world positions used for distance and the budget (when enabled) may be up to N-1 frames stale for non-exempt emitters. 1 = no spread."))
	int32 DetectorTickFrameSpread = 2;

	UPROPERTY(EditAnywhere, Config, Category = "Load Balancing", meta = (DisplayName = "Enable Rate Limiting", ToolTip = "If enabled, the SDK limits how many rays each active detector schedules per frame (see Max Rays Per Frame). Disable to run a detector's full raycast batch in a single frame (lower latency, bigger spikes)."))
	bool AcousticsEnableFrameLimiting = true;

	UPROPERTY(EditAnywhere, Config, Category = "Load Balancing", meta = (DisplayName = "Max Rays Per Frame", EditCondition = "AcousticsEnableFrameLimiting", ClampMin = "1", ClampMax = "50", ToolTip = "When rate limiting is on, each active detector schedules at most this many rays per frame (until its batch is done)."))
	int32 AcousticsMaxRaysPerFrame = 16;

	UPROPERTY(EditAnywhere, Config, Category = "Detection", meta = (DisplayName = "Verification Ray Offset Percent", ClampMin = "0.0", ClampMax = "100.0", ToolTip = "Percentage offset for verification rays"))
	float AcousticsVerificationRayOffsetPercent = 35.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Extra", meta = (DisplayName = "Use Results Interpolation", ToolTip = "Enable smooth transitions between detection results"))
	bool UseResultsInterpolation = true;

	UPROPERTY(EditAnywhere, Config, Category = "Extra", meta = (DisplayName = "Interpolation Duration (seconds)", ClampMin = "0.01", ClampMax = "5.0", EditCondition = "UseResultsInterpolation", ToolTip = "Duration to smoothly blend from old to new results"))
	float InterpolationDuration = 1.0f;

	UPROPERTY(Config)
	float WidthModulatorStrength = 3.0f;

	UPROPERTY(Config)
	float HeightBoostStrength = 0.5f;

	UPROPERTY(Config)
	float BinNoiseThreshold = 0.05f;

	UPROPERTY(EditAnywhere, Config, Category = "Extra", meta = (DisplayName = "Surface Type Absorption Entries", ToolTip = "Set absorption per Physics Surface Type. Configure Surface Types in Project Settings > Physics > Physical Surface; assign them in Physical Materials."))
	TArray<FSurfaceTypeAbsorptionEntry> SurfaceTypeAbsorptionEntries;

	UPROPERTY(EditAnywhere, Config, Category = "Extra", meta = (DisplayName = "Default Absorption", ClampMin = "0.0", ClampMax = "1.0", ToolTip = "Absorption for hits with no Physical Material, Default Surface Type, or surface type not in the list above."))
	float DefaultAbsorptionForUnassignedSurfaces = 0.0f;

	UFUNCTION(BlueprintCallable, Category = "Acoustics Settings")
	static const UExcitedWavesAcousticsSettings* Get();

	void ApplySendBinSoftFloorIfEnabled(FAcousticsReverbSendData& Data) const;

	void ConvertToSDKConfig(struct EWAcoustics::SDK::AcousticsConfig& outConfig) const;

	float GetAbsorptionCoefficient(const struct FHitResult& HitResult) const;
	float GetAbsorptionCoefficientForSurfaceType(EPhysicalSurface SurfaceType) const;

};
