// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

class UExcitedWavesLicenseSettings;

struct FExcitedWavesLicenseCacheData
{
	bool bValid = false;
	FDateTime ValidatedUtc;
	FString LicenseType;
	FString ExpiresAt;
	FString Licensee;
	FString LicenseFingerprint;
};

class EXCITEDWAVESACOUSTICS_API FExcitedWavesLicenseStorage
{
public:
	static FString GetLicenseKeyFilePath();
	static FString GetCacheFilePath();
	static FString ComputeLicenseFingerprint(const FString& LicenseKey);
	static FString ResolveLicenseKey(const UExcitedWavesLicenseSettings* Settings);
	static bool LoadLicenseKeyFromFile(FString& OutKey);
	static void SaveLicenseKeyToFile(const FString& LicenseKey);
	static void ClearPersistedLicenseData();
	static void SaveCache(const FString& LicenseKey, const FString& LicenseType, const FString& ExpiresAt, const FString& Licensee);
	static bool LoadCacheForGrace(FExcitedWavesLicenseCacheData& Out);
	static bool LoadCacheWithinHours(const FString& LicenseKey, int32 Hours, FExcitedWavesLicenseCacheData& Out);
};
