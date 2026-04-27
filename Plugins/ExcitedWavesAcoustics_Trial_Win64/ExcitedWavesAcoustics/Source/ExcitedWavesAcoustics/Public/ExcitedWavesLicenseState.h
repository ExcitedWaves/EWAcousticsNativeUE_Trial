// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "ExcitedWavesLicenseTypes.h"

class EXCITEDWAVESACOUSTICS_API FExcitedWavesLicenseState
{
public:
	static void Reset();
	static void SetFromValidationSuccess(EExcitedWavesLicenseTier Tier, const FString& Licensee, const FString& ExpiresAt, const FString& ReasonEmpty);
	static void SetUnlicensed(const FString& Reason);
	static EExcitedWavesLicenseTier GetTier();
	static bool ShouldBlockCooking();
	static bool IsEditorAcousticsAllowed();
	static FString GetCookBlockLogLine();
	static FString GetStatusHeadline();
	static FString GetLicensee();
	static FString GetExpiresAt();
	static FString GetLastReason();
	static void SetLastHttpSuccessUtc(const FDateTime& Utc);
	static FDateTime GetLastHttpSuccessUtc();
	static void RefreshFromDiskCacheForCook();

private:
	static FCriticalSection Mutex;
	static EExcitedWavesLicenseTier GTier;
	static FString GLicensee;
	static FString GExpiresAt;
	static FString GLastReason;
	static FDateTime GLastHttpSuccessUtc;
};
