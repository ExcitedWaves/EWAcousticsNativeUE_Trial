// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/WeakObjectPtr.h"
#include "IAcousticsAudioAdapter.h"

class UAudioComponent;
class AActor;
class USoundSubmixBase;
class USoundSubmix;
class USubmixEffectExcitedWavesEarlyReflectionsPreset;
class UExcitedWavesNativeAcousticsSettings;
struct FNativeReverbPreset;

class FNativeAcousticsAudioAdapter final : public IAcousticsAudioAdapter
{
public:
	virtual bool Initialize(AActor* Owner) override;
	virtual bool InitializeWithExplicitAudio(AActor* Owner, UAudioComponent* ExplicitAudioComponent) override;
	virtual void Shutdown() override;
	virtual void ApplyReverbSends(const FAcousticsReverbSendData& Data) override;
	virtual void ClearReverbSends() override;
	virtual void UpdateReflections(const FAcousticsReflectUpdateParams& Params) override;
	virtual void ClearReflections() override;
	virtual float GetLastReflectUpdateTime() const override;
	virtual FVector GetLastReflectUpdatePosition() const override;
	virtual void SetLastReflectUpdateTime(float Time) override;
	virtual void SetLastReflectUpdatePosition(const FVector& Pos) override;
	virtual void SetEnableReflections(bool bEnable) override;
	virtual bool IsReflectionsEnabled() const override;
	virtual bool IsAudioSourceActive() const override;
	virtual float GetAcousticsSendsUpdateIntervalSeconds() const override;
#if !UE_BUILD_SHIPPING
	virtual void GatherSubmixSendDebugLines(TArray<FString>& OutLines) const override;
#endif

private:
	UAudioComponent* ResolveAudioComponent();
	void ApplyZeroSends();
	const FNativeReverbPreset* SelectPresetWithHysteresis(float Absorption, const UExcitedWavesNativeAcousticsSettings* Settings);
	void CreateERSubmix();
	void TeardownERSubmixGraph();

	struct FCachedPresetSubmixes
	{
		USoundSubmixBase* Close = nullptr;
		USoundSubmixBase* Mid = nullptr;
		USoundSubmixBase* Far = nullptr;
		USoundSubmixBase* Outdoor = nullptr;
	};
	void EnsureSubmixCacheValid(const UExcitedWavesNativeAcousticsSettings* Settings);
	void EnsureERSubmixConnected();
	void ConditionalSetSubmixSend(UAudioComponent* AC, USoundSubmixBase* Sm, float Level, float MinDelta);

	TWeakObjectPtr<UAudioComponent> m_audio;
	TWeakObjectPtr<AActor> m_owner;
	float m_lastReflectTime = -1.0f;
	FVector m_lastReflectPos = FVector::ZeroVector;
	bool m_bReflectionsEnabled = false;

	int32 m_currentPresetIndex = -1;
	int32 m_previousPresetIndex = -1;
	float m_crossfadeAlpha = 1.0f;
	double m_lastApplyTime = 0.0;

	TArray<FCachedPresetSubmixes> m_submixCache;

	TArray<TWeakObjectPtr<USoundSubmixBase>> m_activeSendSubmixes;

	TMap<USoundSubmixBase*, float> m_lastSubmixSendLevels;

	USoundSubmix* m_erSubmix = nullptr;
	USubmixEffectExcitedWavesEarlyReflectionsPreset* m_erPreset = nullptr;
	bool m_bERSubmixConnected = false;
	bool m_bLoggedERConnectFailure = false;
	bool m_bLoggedSubmixSendCap = false;
};
