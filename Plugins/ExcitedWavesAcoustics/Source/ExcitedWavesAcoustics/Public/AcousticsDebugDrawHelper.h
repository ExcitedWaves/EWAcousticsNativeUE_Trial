// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "ExcitedWavesAcousticsSDK/Public/AcousticsTypes.h"
#include "ExcitedWavesAcousticsSDK/Public/AcousticsDebugTypes.h"

class UExcitedWavesAcousticsSettings;

struct FDebugTextContext
{
	EWAcoustics::SDK::DetectorId DetectorId = 0;
	bool bIsListener = false;

	const EWAcoustics::SDK::DebugData* DebugData = nullptr;
	const EWAcoustics::SDK::AcousticsResults* AnalysisResults = nullptr;
	const EWAcoustics::SDK::AcousticsConfig* Config = nullptr;

	float LastSendClose = 0.0f;
	float LastSendMid = 0.0f;
	float LastSendFar = 0.0f;
	float LastSendOutdoor = 0.0f;
	float LastDistanceToListener = 0.0f;
	float LastReverbMultiplier = 1.0f;
	float LastCrossRoomContrib = 0.0f;
	float OutdoorReverbPercentage = 0.0f;

	EWAcoustics::SDK::DetectorId ListenerDetectorId = 0;
	const EWAcoustics::SDK::AcousticsResults* ListenerResults = nullptr;
	FVector ListenerPosition = FVector::ZeroVector;

#if !UE_BUILD_SHIPPING
	TArray<FString> SubmixSendDebugLines;
#endif
};

class EXCITEDWAVESACOUSTICS_API FAcousticsDebugDrawHelper
{
public:
	static void DrawDebugRays(
		UWorld* World,
		const EWAcoustics::SDK::DebugData& DebugData,
		const UExcitedWavesAcousticsSettings* Settings,
		const FVector& ViewerPosition,
		bool bIgnoreInDebugDraw);

	static void DrawDebugHitPoints(
		UWorld* World,
		const EWAcoustics::SDK::DebugData& DebugData,
		const UExcitedWavesAcousticsSettings* Settings,
		const FVector& ViewerPosition,
		bool bIgnoreInDebugDraw);

	static FString CreateDebugText(
		const FDebugTextContext& Context,
		const UExcitedWavesAcousticsSettings* Settings);

	static FString GetStateString(EWAcoustics::SDK::EDetectorState State);

	static bool ShouldDrawAtPosition(
		const FVector& Position,
		const FVector& ViewerPosition,
		const UExcitedWavesAcousticsSettings* Settings,
		bool bIgnoreInDebugDraw);
};
