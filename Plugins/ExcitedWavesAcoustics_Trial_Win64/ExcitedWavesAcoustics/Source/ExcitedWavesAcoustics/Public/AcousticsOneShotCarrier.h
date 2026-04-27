// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AcousticsOneShotCarrier.generated.h"

class USceneComponent;

/**
 * Minimal hidden actor used by UAcousticsSubsystem::PlaySoundAtLocationWithAcoustics.
 *
 * Serves as an owner anchor for:
 *   - the spawned UAudioComponent (needed by FNativeAcousticsAudioAdapter for world/submix lifecycle), and/or
 *   - a temporary UAcousticsDetectorComponent when bCreateTempDetector is true.
 *
 * The subsystem owns the lifetime and destroys the carrier once the one-shot playback ends.
 */
UCLASS(NotBlueprintable, NotPlaceable, Hidden)
class EXCITEDWAVESACOUSTICS_API AAcousticsOneShotCarrier : public AActor
{
	GENERATED_BODY()

public:
	AAcousticsOneShotCarrier();

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Acoustics")
	TObjectPtr<USceneComponent> RootScene;
};
