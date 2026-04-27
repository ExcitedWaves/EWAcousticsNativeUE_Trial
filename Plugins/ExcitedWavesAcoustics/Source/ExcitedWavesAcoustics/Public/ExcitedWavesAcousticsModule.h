// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

EXCITEDWAVESACOUSTICS_API DECLARE_LOG_CATEGORY_EXTERN(LogEWAcoustics, Log, All);

class FExcitedWavesAcousticsModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
