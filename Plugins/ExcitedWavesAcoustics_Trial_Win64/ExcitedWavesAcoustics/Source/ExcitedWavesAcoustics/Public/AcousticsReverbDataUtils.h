// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "ExcitedWavesAcousticsSDK/Public/AcousticsTypes.h"
#include "IAcousticsAudioAdapter.h"

struct FAcousticsReverbDataBuildOptions
{
	bool bIgnoreCrossRoom = false;
	bool bIgnoreDirectToReverbRatio = false;
};

struct EXCITEDWAVESACOUSTICS_API FAcousticsReverbDataUtils
{
	static void BuildFromProcessedSends(
		const EWAcoustics::SDK::ProcessedSendLevels& Sends,
		const FAcousticsReverbDataBuildOptions& Opts,
		FAcousticsReverbSendData& OutData);
};
