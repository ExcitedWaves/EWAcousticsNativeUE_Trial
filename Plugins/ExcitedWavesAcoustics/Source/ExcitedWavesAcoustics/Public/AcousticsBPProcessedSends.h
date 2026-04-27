// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "AcousticsBPProcessedSends.generated.h"

USTRUCT(BlueprintType)
struct FAcousticsBPProcessedSendGroup
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Acoustics")
	float CloseRev = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Acoustics")
	float MidRev = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Acoustics")
	float FarRev = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Acoustics")
	float OutdoorRev = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Acoustics")
	float MeanAbsorption = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Acoustics")
	float PresetAbsorption = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Acoustics")
	float CloseAbsorption = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Acoustics")
	float MidAbsorption = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Acoustics")
	float FarAbsorption = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Acoustics")
	float OutdoorAbsorption = 0.0f;
};

USTRUCT(BlueprintType)
struct FAcousticsBPProcessedSendLevels
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Acoustics")
	FAcousticsBPProcessedSendGroup Emitter;

	UPROPERTY(BlueprintReadOnly, Category = "Acoustics")
	FAcousticsBPProcessedSendGroup CrossRoom;

	UPROPERTY(BlueprintReadOnly, Category = "Acoustics")
	float DistanceToListener = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Acoustics")
	float CrossRoomContribution = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Acoustics")
	float ReverbMultiplier = 1.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Acoustics")
	bool bHasListenerData = false;

	UPROPERTY(BlueprintReadOnly, Category = "Acoustics")
	bool bHasValidResults = false;
};
