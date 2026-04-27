// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "ExcitedWavesAcousticsSDK/Public/AcousticsTypes.h"

class UAcousticsDetectorComponent;
class AActor;

enum class EAcousticsDetectorBinding : uint8
{
	Component,
	External,
};

struct FAcousticsExternalDetectorParams
{
	FVector InitialWorldLocation = FVector::ZeroVector;
	TWeakObjectPtr<AActor> RaycastIgnoreActor;
};

struct FAcousticsDetectorRecord
{
	EAcousticsDetectorBinding Binding = EAcousticsDetectorBinding::Component;
	TWeakObjectPtr<UAcousticsDetectorComponent> Component;
	FVector ExternalWorldLocation = FVector::ZeroVector;
	TWeakObjectPtr<AActor> RaycastIgnoreActor;

	FVector CachedWorldPosition = FVector::ZeroVector;
	float CachedDistanceToListenerCm = 0.0f;
	bool bBudgetEvaluationPending = false;
};
