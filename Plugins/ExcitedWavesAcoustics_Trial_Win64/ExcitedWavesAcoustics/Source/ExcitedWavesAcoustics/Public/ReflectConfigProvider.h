// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "ExcitedWavesAcousticsSDK/Public/AcousticsTypes.h"
#include "Templates/Function.h"

class EXCITEDWAVESACOUSTICS_API FReflectConfigProvider
{
public:
	using FillerFunc = TFunction<void(EWAcoustics::SDK::AcousticsConfig::ImageSourceConfig&)>;

	static void Register(FillerFunc Func);
	static void Unregister();
	static void Fill(EWAcoustics::SDK::AcousticsConfig::ImageSourceConfig& OutConfig);

private:
	static FillerFunc s_provider;
};
