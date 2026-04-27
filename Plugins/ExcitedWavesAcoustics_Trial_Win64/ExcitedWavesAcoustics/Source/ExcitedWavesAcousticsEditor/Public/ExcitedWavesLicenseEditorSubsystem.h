// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "Interfaces/IHttpRequest.h"
#include "ExcitedWavesLicenseEditorSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogExcitedWavesLicense, Log, All);

UCLASS()
class EXCITEDWAVESACOUSTICSEDITOR_API UExcitedWavesLicenseEditorSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void PumpFrame();
	void RequestRevalidateFromSettings();
	FText GetDetailedStatusText() const;

private:
	void RunInitialValidation();
	void StartHttpValidation(const FString& LicenseKey);
	void HandleHttpComplete(FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void ApplySuccessJson(const TSharedPtr<class FJsonObject>& Obj);
	void ApplyFailureOrOffline(const FString& Reason, bool bTryGraceCache);
	void ApplyTierFromCacheData(const struct FExcitedWavesLicenseCacheData& Cache);
	void NotifyInfo(const FString& Msg);
	void NotifyWarning(const FString& Msg);
	void SendTelemetry(const FString& EventName, int32 OptionalDurationSeconds = -1);
	void FlushTelemetrySessionEnd();
	void BindPreExit();
	void UnbindPreExit();
	void HandlePreExit();
	void RefreshAcousticsWithLicense();

	FString MachineId;
	FString PluginVersionString;
	FString ActiveLicenseKey;
	FString SessionId;
	double AccumulatedEditorSeconds = 0.0;
	double LastHeartbeatWallTime = 0.0;
	bool bTelemetrySessionStarted = false;
	bool bHadSuccessfulLicenseThisSession = false;
	FDelegateHandle MapOpenedHandle;
	FDelegateHandle BeginPIEHandle;
};
