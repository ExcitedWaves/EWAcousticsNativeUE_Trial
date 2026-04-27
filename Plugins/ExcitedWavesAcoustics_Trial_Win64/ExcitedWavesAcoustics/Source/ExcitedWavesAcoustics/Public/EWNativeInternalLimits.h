// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

namespace ExcitedWaves::Native::Private
{
inline constexpr int32 EWMaxUniqueSubmixSendsPerFrame = 32;
inline constexpr int32 EWMaxEarlyReflectionTaps = 8;
inline constexpr float EWDefaultERWetLinear = 0.45f;
inline constexpr float EWDefaultERGainSmoothingSeconds = 0.06f;
inline constexpr float EWDefaultERCrossfadeSeconds = 0.015f;
}
