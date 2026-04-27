// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "AcousticsTypes.h"
#include "AcousticsDebugTypes.h"
#include <unordered_map>
#include <memory>
#include <cstddef>
#include <vector>
#include <cstdint>

namespace EWAcoustics
{
	namespace SDK
	{

		class AcousticsDetector;

		class AcousticsDetectorManager
		{
		public:
			EWSDK_API AcousticsDetectorManager();
			EWSDK_API ~AcousticsDetectorManager();

			AcousticsDetectorManager(const AcousticsDetectorManager&) = delete;
			AcousticsDetectorManager& operator=(const AcousticsDetectorManager&) = delete;

			EWSDK_API DetectorId CreateDetector(const AcousticsConfig& config);
			EWSDK_API void DestroyDetector(DetectorId detectorId);

			EWSDK_API void UpdateDetector(DetectorId detectorId, float deltaTime, uint32_t frameNumber);
			EWSDK_API void SetDetectorListenerDistanceCm(DetectorId detectorId, float distanceCm);
			EWSDK_API void SetDetectorPosition(DetectorId detectorId, const Vector3& position);
			EWSDK_API void ForceRestartDetector(DetectorId detectorId);
			EWSDK_API void UpdateAllDetectorsConfig(const AcousticsConfig& config);
			EWSDK_API void UpdateDetectorConfig(DetectorId detectorId, const AcousticsConfig& config);
			EWSDK_API void SetAllDetectorsRayScanningMethod(ERayScanningMethod method);

			EWSDK_API void SetListenerPosition(const Vector3& position);
			EWSDK_API const Vector3& GetListenerPosition() const;
			EWSDK_API void SetListenerOrientation(const Vector3& forward, const Vector3& up);
			EWSDK_API void SetLODConfig(const DistanceLODConfig& config);

			EWSDK_API void SetListenerDetectorId(DetectorId detectorId);
			EWSDK_API DetectorId GetListenerDetectorId() const;
			EWSDK_API void SetListenerBlendConfig(const ListenerBlendConfig& config);

			EWSDK_API ProcessedSendLevels GetDetectorProcessedSends(DetectorId detectorId) const;

			EWSDK_API const ProbeRequests& GetDetectorRayRequests(DetectorId detectorId) const;
			EWSDK_API void AddProbeResult(DetectorId detectorId, const ProbeResult& result);

			EWSDK_API const AcousticsResults* GetDetectorResults(DetectorId detectorId) const;
			EWSDK_API const DebugData* GetDetectorDebugData(DetectorId detectorId) const;

			EWSDK_API bool DetectorHasNewDebugData(DetectorId detectorId) const;

			EWSDK_API const std::vector<ProbeResult>* GetDetectorSideProbeResults(DetectorId detectorId) const;
			EWSDK_API const std::vector<ProbeResult>* GetDetectorUpProbeResults(DetectorId detectorId) const;
			EWSDK_API const std::vector<ImageSource>* GetDetectorImageSources(DetectorId detectorId) const;
			EWSDK_API void MarkDetectorDebugDataConsumed(DetectorId detectorId);

			EWSDK_API size_t GetDetectorCount() const;
			EWSDK_API EDetectorState GetDetectorState(DetectorId detectorId) const;
			EWSDK_API bool IsDetectorReflectCulled(DetectorId detectorId) const;
			EWSDK_API uint32_t GetDetectorDetectionGeneration(DetectorId detectorId) const;

			EWSDK_API void ResetGlobalFrameLimiting(uint32_t frameNumber);
			EWSDK_API uint32_t ReserveRaysForFrame(uint32_t requestedRays, uint32_t frameNumber, uint8_t maxRaysPerFrame);
			EWSDK_API uint32_t GetGlobalRaysScheduledThisFrame() const;

			EWSDK_API void SetCurrentTimeSeconds(double seconds);
			EWSDK_API double GetCurrentTimeSeconds() const;

			EWSDK_API void BeginFrame(uint32_t frameNumber);

		private:
			AcousticsDetector* GetDetector(DetectorId detectorId) const;
			void ApplyDistanceLOD(DetectorId detectorId, const Vector3& emitterPosition);

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif
		private:
			std::unordered_map<DetectorId, std::unique_ptr<AcousticsDetector>> m_detectors;
			Vector3 m_listenerPosition;
			DistanceLODConfig m_lodConfig;
			DetectorId m_listenerDetectorId = 0;
			ListenerBlendConfig m_listenerBlendConfig;

			uint32_t m_globalRaysScheduledThisFrame = 0;
			uint32_t m_globalLastFrameNumber = 0;

			Vector3 m_listenerForward;
			Vector3 m_listenerUp;

			double m_currentTimeSeconds = 0.0;

			static const ProbeRequests s_emptyRayRequests;
			static const AcousticsResults s_emptyResults;
			static const DebugData s_emptyDebugData;
		};
#ifdef _MSC_VER
#pragma warning(pop)
#endif

	} // namespace SDK
} // namespace EWAcoustics

extern "C"
{
	EWSDK_API EWAcoustics::SDK::AcousticsDetectorManager* EWAcoustics_CreateAcousticsDetectorManager();
	EWSDK_API void EWAcoustics_DestroyAcousticsDetectorManager(EWAcoustics::SDK::AcousticsDetectorManager* Manager);
}

namespace EWAcoustics
{
	namespace SDK
	{

		inline AcousticsDetectorManager* CreateAcousticsDetectorManager()
		{
			return EWAcoustics_CreateAcousticsDetectorManager();
		}

		inline void DestroyAcousticsDetectorManager(AcousticsDetectorManager* Manager)
		{
			EWAcoustics_DestroyAcousticsDetectorManager(Manager);
		}

		struct AcousticsDetectorManagerDeleter
		{
			void operator()(AcousticsDetectorManager* Manager) const noexcept
			{
				DestroyAcousticsDetectorManager(Manager);
			}
		};

	} // namespace SDK
} // namespace EWAcoustics
