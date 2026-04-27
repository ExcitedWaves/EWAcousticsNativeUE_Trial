// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "ExcitedWavesAcousticsSDK/Public/AcousticsTypes.h"
#include "Templates/Function.h"

class EXCITEDWAVESACOUSTICS_API FAcousticsConfigOverrides
{
public:
	using OverrideFunc = TFunction<void(EWAcoustics::SDK::AcousticsConfig&)>;

	static void Register(OverrideFunc Func);
	static void Unregister();
	static void Apply(EWAcoustics::SDK::AcousticsConfig& Config);

private:
	static OverrideFunc s_override;
};
