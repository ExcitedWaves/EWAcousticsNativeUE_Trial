// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Chaos/ChaosGameplayEventDispatcher.h"
#include "DestructionAcousticsSubsystem.generated.h"

class UAcousticsDetectorComponent;

UCLASS()
class EXCITEDWAVESACOUSTICSCHAOS_API UDestructionAcousticsSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UDestructionAcousticsSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override { return true; }

	/**
	 * Manual registration of detector (optional: auto-discovery works)
	 * Returns true if registered successfully
	 */
	UFUNCTION(BlueprintCallable, Category = "Destruction Acoustics")
	bool RegisterDetector(UAcousticsDetectorComponent* Detector);

	UFUNCTION(BlueprintCallable, Category = "Destruction Acoustics")
	void UnregisterDetector(UAcousticsDetectorComponent* Detector);

	/**
	 * Force scan for new Geometry Collections in world
	 * Called automatically on Initialize and periodically
	 */
	UFUNCTION(BlueprintCallable, Category = "Destruction Acoustics")
	void ScanForGeometryCollections();

	/**
	 * Force scan for new Acoustics Detectors in world
	 * Called automatically on Initialize and periodically
	 */
	UFUNCTION(BlueprintCallable, Category = "Destruction Acoustics")
	void ScanForAcousticsDetectors();

	UFUNCTION(BlueprintCallable, Category = "Destruction Acoustics")
	int32 GetTrackedGeometryCollectionCount() const { return TrackedGeometryCollections.Num(); }

	UFUNCTION(BlueprintCallable, Category = "Destruction Acoustics")
	int32 GetRegisteredDetectorCount() const { return RegisteredDetectors.Num(); }

protected:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

private:
	/**
	 * Called when any tracked Geometry Collection breaks
	 */
	UFUNCTION()
	void OnChaosBreakEvent(const FChaosBreakEvent& BreakEvent);

	/**
	 * Called when any tracked Geometry Collection pieces are removed from world
	 */
	UFUNCTION()
	void OnChaosRemovalEvent(const FChaosRemovalEvent& RemovalEvent);

	/**
	 * Called after RecalculationDelay timer expires to batch-notify detectors
	 */
	void ExecuteDelayedNotification();

	void SubscribeToGeometryCollection(UGeometryCollectionComponent* GeoCollection);
	void UnsubscribeFromGeometryCollection(UGeometryCollectionComponent* GeoCollection);
	void CleanupInvalidComponents();
	void LoadSettings();
	void ExecuteMultiIterationDetection(UAcousticsDetectorComponent* Detector, int32 CurrentIteration);
	void OnIterationTimerExpired(TWeakObjectPtr<UAcousticsDetectorComponent> WeakDetector);
	void QueueNotificationsInRadius(const FVector& EventLocation, bool bApplyRemovalCooldown);
	void ScheduleDelayedNotification();

private:
	UPROPERTY()
	TArray<UGeometryCollectionComponent*> TrackedGeometryCollections;

	UPROPERTY()
	TArray<UAcousticsDetectorComponent*> RegisteredDetectors;

	float NotificationRadius;
	float MinBreakMagnitudeToTrigger;
	float PerDetectorCooldown;
	float RecalculationDelay;
	float AutoScanInterval;

	bool bMultiIterationDetection;
	int32 MaxIterations;
	float IterationInterval;

	bool bEnableRemovalEvents;
	float RemovalEventCooldown;

	TMap<TWeakObjectPtr<UAcousticsDetectorComponent>, float> DetectorLastNotificationTime;
	FTimerHandle AutoScanTimerHandle;
	FTimerHandle DelayedNotificationTimerHandle;
	bool bIsInitialized = false;

	TSet<TWeakObjectPtr<UAcousticsDetectorComponent>> PendingNotifications;

	TMap<TWeakObjectPtr<UAcousticsDetectorComponent>, int32> DetectorIterationCount;
	TMap<TWeakObjectPtr<UAcousticsDetectorComponent>, FTimerHandle> DetectorIterationTimers;

	TMap<TWeakObjectPtr<UAcousticsDetectorComponent>, float> DetectorLastRemovalTime;
};
