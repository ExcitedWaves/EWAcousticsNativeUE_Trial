// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/Engine.h"
#include "ExcitedWavesAcousticsSDK/Public/AcousticsTypes.h"
#include "AcousticsBenchmarkComponent.generated.h"

class AActor;
class UAcousticsDetectorComponent;
class USceneComponent;
class UAcousticsSubsystem;

UENUM(BlueprintType)
enum class EBenchmarkMovementPattern : uint8
{
	Static,
	Circle,
	Linear,
	Random,
	Grid
};

UENUM(BlueprintType)
enum class EBenchmarkTriggerDistribution : uint8
{
	Spread,
	Burst
};

USTRUCT(BlueprintType)
struct FBenchmarkMetrics
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	float CurrentFPS = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	float AverageFPS = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	float MinFPS = 9999.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	float MaxFPS = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	float FrameTimeMS = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	float AverageFrameTimeMS = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	float PeakFrameTimeMS = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	int32 ActiveDetectorCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	int32 RaycastsPerFrame = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	float AverageRaycastsPerFrame = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	float MemoryUsageMB = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	float BenchmarkDuration = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	int32 FrameDrops = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	int32 SevereFrameDrops = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	int32 TriggersFired = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	int32 LastTriggerSize = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	float LastTriggerCostMS = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	float MaxTriggerCostMS = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Metrics")
	int32 PeakRaycastsPerFrame = 0;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EXCITEDWAVESACOUSTICSBENCHMARK_API UAcousticsBenchmarkComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAcousticsBenchmarkComponent();
	virtual ~UAcousticsBenchmarkComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Benchmark")
	void StartBenchmark();

	UFUNCTION(BlueprintCallable, Category = "Benchmark")
	void StopBenchmark();

	UFUNCTION(BlueprintCallable, Category = "Benchmark")
	void ResetBenchmark();

	UFUNCTION(BlueprintCallable, Category = "Benchmark")
	FBenchmarkMetrics GetCurrentMetrics() const { return m_currentMetrics; }

	UFUNCTION(BlueprintCallable, Category = "Benchmark")
	bool IsBenchmarkRunning() const { return m_bIsRunning; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Setup", meta = (ClampMin = "1", ClampMax = "1000", ToolTip = "Maximum 1000 concurrent benchmark detectors/emitters in this run."))
	int32 ActorCount = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Setup", meta = (EditCondition = "!bSpawnMinimalDetectorActors", EditConditionHides, ToolTip = "Prefab class for each benchmark emitter. Ignored when Minimal Detector Actors is enabled."))
	TSubclassOf<AActor> ActorTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Setup", meta = (ToolTip = "Registers detectors directly via UAcousticsSubsystem::RegisterExternalDetector (no spawned benchmark actors). Positions are pushed before the subsystem tick when MovementPattern is non-static; Static uses initial registration only. Triggers call ForceRestartDetector on sampled ids. Use for isolating SDK analysis and raycast cost from engine actor overhead."))
	bool bSpawnMinimalDetectorActors = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Setup")
	bool bSpawnInGrid = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Setup", meta = (ToolTip = "Disable collision, replication, actor-level Tick on spawned actors. Safe default: leaves component ticks alone so UAcousticsDetectorComponent still runs analysis / debug draw."))
	bool bOptimizeActors = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Setup", meta = (EditCondition = "bOptimizeActors", ToolTip = "Also unregister every component Tick on spawned actors. This removes tick-dispatch overhead but DISABLES acoustics analysis and debug draw on the spawned detectors (they will not tick at all). Use for pure engine-overhead measurement runs; leave false for realistic benchmarking of the acoustics system."))
	bool bDisableComponentTicks = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Setup")
	bool bAutoStart = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Setup", meta = (ClampMin = "0"))
	float AutoStopDuration = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Spawn Volume", meta = (ClampMin = "0"))
	float SpawnVolumeWidth = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Spawn Volume", meta = (ClampMin = "0"))
	float SpawnVolumeDepth = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Spawn Volume", meta = (ClampMin = "0"))
	float SpawnVolumeHeight = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Movement", meta = (ToolTip = "Pattern for actors that move. Static = no movement (recommended for pure acoustics CPU measurement; use Triggers below to simulate re-evaluation events). Non-static patterns include engine transform-propagation overhead (USceneComponent updates), which at high actor counts dominates over the acoustics cost."))
	EBenchmarkMovementPattern MovementPattern = EBenchmarkMovementPattern::Static;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Movement", meta = (ClampMin = "0", ToolTip = "How many of the first N spawned actors actually move (remaining ones stay static). 0 = all actors follow MovementPattern (legacy behaviour). Use a small number to simulate 'only NPCs move, rest of the world is static props'. Has no effect when MovementPattern is Static."))
	int32 MovingEmitterCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Movement", meta = (ClampMin = "0", EditCondition = "MovementPattern != EBenchmarkMovementPattern::Static", EditConditionHides))
	float MovementSpeedMin = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Movement", meta = (ClampMin = "0", EditCondition = "MovementPattern != EBenchmarkMovementPattern::Static", EditConditionHides))
	float MovementSpeedMax = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Movement", meta = (ClampMin = "5.0", ClampMax = "240.0", EditCondition = "MovementPattern != EBenchmarkMovementPattern::Static", EditConditionHides, ToolTip = "Rate at which spawned actors are moved. Lower values (e.g. 30) drastically reduce the cost of USceneComponent transform propagation across N actors at negligible visual impact. Step size is accumulated from engine DeltaTime so actor speeds stay frame-rate independent."))
	float MovementUpdateHz = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Movement", meta = (ClampMin = "0", EditCondition = "MovementPattern == EBenchmarkMovementPattern::Circle", EditConditionHides))
	float CircleRadius = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Movement", meta = (ClampMin = "0", EditCondition = "MovementPattern == EBenchmarkMovementPattern::Linear", EditConditionHides))
	float LinearDistance = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Movement", meta = (ClampMin = "1", EditCondition = "MovementPattern == EBenchmarkMovementPattern::Grid", EditConditionHides))
	int32 GridSize = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Movement", meta = (ClampMin = "0", EditCondition = "MovementPattern == EBenchmarkMovementPattern::Grid", EditConditionHides))
	float GridOscillationAmplitude = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Movement", meta = (ClampMin = "0", EditCondition = "MovementPattern == EBenchmarkMovementPattern::Grid", EditConditionHides))
	float GridOscillationSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Triggers", meta = (ToolTip = "When enabled, the benchmark periodically calls ForceRecalculation() on a random subset of spawned detectors so they restart analysis (same idea as static-mode trigger batches). Pair with MovementPattern=Static to measure acoustics CPU without transform overhead."))
	bool bEnableTriggers = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Triggers", meta = (ClampMin = "0.05", EditCondition = "bEnableTriggers", ToolTip = "Seconds between trigger events."))
	float TriggerIntervalSec = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Triggers", meta = (ClampMin = "1", ClampMax = "1000", EditCondition = "bEnableTriggers", ToolTip = "Number of emitters to re-evaluate per trigger (random sample without replacement). Capped at 1000; also clamped to the active actor/detector count at runtime."))
	int32 EmittersPerTrigger = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Triggers", meta = (EditCondition = "bEnableTriggers", ToolTip = "Spread: distribute calls evenly across the interval (measures steady-state cost, smooth frame times). Burst: fire the whole trigger in a single frame (measures worst-case hitch when many events happen simultaneously, e.g. an explosion)."))
	EBenchmarkTriggerDistribution TriggerDistribution = EBenchmarkTriggerDistribution::Spread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Display")
	bool bShowOnScreenMetrics = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Display", meta = (ClampMin = "0.01"))
	float MetricsUpdateInterval = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Display")
	bool bLogMetrics = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Display", meta = (ClampMin = "1.0"))
	float LogInterval = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Display")
	bool bScaleByDistance = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Display", meta = (ClampMin = "100", EditCondition = "bScaleByDistance", EditConditionHides))
	float ScaleReferenceDistance = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Spawn Volume|Debug")
	bool bDrawSpawnVolumeAtRuntime = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Spawn Volume|Debug", meta = (ClampMin = "0.5"))
	float SpawnVolumeDebugLineThickness = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Benchmark|Spawn Volume|Debug")
	FColor SpawnVolumeDebugColor = FColor::Emerald;

	UPROPERTY(BlueprintReadOnly, Category = "Benchmark|Results")
	FBenchmarkMetrics m_currentMetrics;

private:
	void GenerateActors();
	void DestroyActors();
	void FlushExternalBenchmarkDetectorPositions(UAcousticsSubsystem* Subsystem);
	void UpdateActorMovement(float DeltaTime);
	void UpdateActorScaling();
	void UpdateMetrics(float DeltaTime);
	void UpdateTriggers(float DeltaTime);
	int32 FireTriggerBatch(int32 BatchSize);
#if !UE_BUILD_SHIPPING
	void DrawOnScreenMetrics();
	void DrawSpawnVolume() const;
#endif
	FVector CalculateSpawnLocalPosition(int32 ActorIndex) const;
	FVector CalculateTargetPosition(int32 ActorIndex, float DeltaTime, const FVector& OwnerLocation, const FVector& CurrentPosition);
	FVector GetRandomBoxOffsetUniform() const;
	float GetMemoryUsageMB() const;
	int32 GetEffectiveMovingCount() const;

	bool m_bIsRunning = false;
	int32 m_cachedDetectorCount = 0;

	UPROPERTY()
	TArray<AActor*> m_spawnedActors;

	struct FActorMovementData
	{
		TWeakObjectPtr<USceneComponent> Root;
		FVector StartPosition;
		FVector CurrentTarget;
		float Angle = 0.0f;
		float LinearProgress = 0.0f;
		bool bMovingForward = true;
		float Speed = 200.0f;
	};

	TArray<FActorMovementData> m_movementData;

	TArray<EWAcoustics::SDK::DetectorId> m_externalDetectorIds;
	TArray<FVector> m_externalDetectorWorldPositions;

	float m_metricsUpdateTimer = 0.0f;
	float m_movementAccumulator = 0.0f;
	float m_logTimer = 0.0f;
	float m_benchmarkStartTime = 0.0f;
	TArray<float> m_fpsHistory;
	TArray<float> m_frameTimeHistory;
	TArray<int32> m_raycastHistory;

	float m_triggerAccumulator = 0.0f;
	float m_spreadAccumulator = 0.0f;

	static constexpr float FrameDropThreshold30FPS = 33.33f;
	static constexpr float SevereFrameDropThreshold = 50.0f;
};
