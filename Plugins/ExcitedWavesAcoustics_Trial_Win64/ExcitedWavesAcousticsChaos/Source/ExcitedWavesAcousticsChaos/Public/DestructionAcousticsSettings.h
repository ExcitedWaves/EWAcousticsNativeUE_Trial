// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DestructionAcousticsSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Excited Waves Acoustics Chaos"))
class EXCITEDWAVESACOUSTICSCHAOS_API UDestructionAcousticsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UDestructionAcousticsSettings();

	virtual FName GetCategoryName() const override;
#if WITH_EDITOR
	virtual FText GetSectionDescription() const override;
#endif

	UFUNCTION(BlueprintCallable, Category = "Destruction Acoustics Settings")
	static const UDestructionAcousticsSettings* Get();

	UPROPERTY(EditAnywhere, Config, Category = "Destruction", meta = (DisplayName = "Notification Radius (cm)", ClampMin = "100.0", ClampMax = "20000.0", ToolTip = "Radius around destruction event to search for detectors"))
	float DestructionNotificationRadius = 5000.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Destruction", meta = (DisplayName = "Min Break Magnitude", ClampMin = "0.0", ToolTip = "Minimum break magnitude (mass * velocity) to trigger notifications"))
	float DestructionMinBreakMagnitude = 100.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Destruction", meta = (DisplayName = "Per-Detector Cooldown (seconds)", ClampMin = "0.0", ClampMax = "5.0", ToolTip = "Cooldown per detector to prevent spam"))
	float DestructionPerDetectorCooldown = 0.5f;

	UPROPERTY(EditAnywhere, Config, Category = "Destruction", meta = (DisplayName = "Recalculation Delay (seconds)", ClampMin = "0.0", ClampMax = "10.0", ToolTip = "Delay before recalculation after destruction (to let geometry stabilize)"))
	float DestructionRecalculationDelay = 0.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Destruction", meta = (DisplayName = "Auto-Scan Interval (seconds)", ClampMin = "0.0", ClampMax = "30.0", ToolTip = "How often to scan for new destructible objects (0 = only on level start)"))
	float DestructionAutoScanInterval = 5.0f;

	UPROPERTY(EditAnywhere, Config, Category = "Destruction", meta = (DisplayName = "Multi-Iteration Detection", ToolTip = "Enable multiple detection iterations after destruction for smoother results"))
	bool DestructionMultiIterationDetection = true;

	UPROPERTY(EditAnywhere, Config, Category = "Destruction", meta = (DisplayName = "Max Iterations", ClampMin = "1", ClampMax = "10", EditCondition = "DestructionMultiIterationDetection", ToolTip = "Maximum number of detection iterations after destruction"))
	int32 DestructionMaxIterations = 3;

	UPROPERTY(EditAnywhere, Config, Category = "Destruction", meta = (DisplayName = "Iteration Interval (seconds)", ClampMin = "0.1", ClampMax = "2.0", EditCondition = "DestructionMultiIterationDetection", ToolTip = "Time between detection iterations"))
	float DestructionIterationInterval = 0.5f;

	UPROPERTY(EditAnywhere, Config, Category = "Destruction", meta = (DisplayName = "Enable Removal Events", ToolTip = "Also trigger detection when destructible pieces are removed from world"))
	bool DestructionEnableRemovalEvents = true;

	UPROPERTY(EditAnywhere, Config, Category = "Destruction", meta = (DisplayName = "Removal Event Cooldown (seconds)", ClampMin = "0.0", ClampMax = "5.0", EditCondition = "DestructionEnableRemovalEvents", ToolTip = "Cooldown for removal events to prevent spam"))
	float DestructionRemovalEventCooldown = 0.2f;

};
