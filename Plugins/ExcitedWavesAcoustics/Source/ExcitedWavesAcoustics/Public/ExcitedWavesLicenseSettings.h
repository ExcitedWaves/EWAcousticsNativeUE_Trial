// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ExcitedWavesLicenseSettings.generated.h"

UCLASS(Config = Editor, DefaultConfig, meta = (DisplayName = "Excited Waves"))
class EXCITEDWAVESACOUSTICS_API UExcitedWavesLicenseSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UExcitedWavesLicenseSettings();

	virtual FName GetCategoryName() const override;
#if WITH_EDITOR
	virtual FText GetSectionDescription() const override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY(EditAnywhere, Config, Category = "License", meta = (DisplayName = "License Key", PasswordField = true, ToolTip = "Paste the JWT license string. Also stored under Saved/ExcitedWavesLicense.json when changed."))
	FString LicenseKey;

	UPROPERTY(EditAnywhere, Config, Category = "License", meta = (DisplayName = "Send usage statistics", ToolTip = "If enabled, sends anonymized session events (start, periodic heartbeat, end) to the same server with action=telemetry."))
	bool bSendUsageStatistics = false;

	UFUNCTION(BlueprintCallable, Category = "ExcitedWaves|License")
	static const UExcitedWavesLicenseSettings* Get();
};
