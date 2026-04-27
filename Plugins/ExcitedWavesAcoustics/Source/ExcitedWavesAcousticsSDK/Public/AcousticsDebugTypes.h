// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "AcousticsTypes.h"

namespace EWAcoustics
{
	namespace SDK
	{

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif
		struct EWSDK_API DebugData
		{
			struct EWSDK_API RayData
			{
				Vector3 origin;
				Vector3 direction;
				Vector3 hitPosition;
				float maxDistance = 0.0f;
				float timestamp = 0.0f;
				float outlierWeight = 1.0f;
				RayId rayId = 0;
				ERayType rayType = ERayType::None;
				bool hit = false;
				bool requiresVerification = false;
				bool hasVerificationHit = false;
				bool isOutlier = false;

				RayData() = default;
			};

			struct EWSDK_API AnalysisData
			{
				AcousticsResults results;
				std::vector<ProbeResult> outliers;
				std::vector<ProbeResult> verifiedRays;
				uint8_t totalProbeRays = 0;
				uint8_t totalVerificationRays = 0;
				bool analysisComplete = false;

				AnalysisData() = default;
			};

			struct EWSDK_API FrameLimitingStats
			{
				uint32_t pendingRaysCount = 0;
				uint32_t totalRaysGenerated = 0;
				uint32_t raysScheduledThisFrame = 0;
				bool frameLimitingEnabled = false;

				FrameLimitingStats() = default;
			};

			std::vector<RayData> rays;
			AnalysisData analysis;
			FrameLimitingStats frameLimiting;
			EDetectorState currentState = EDetectorState::Inactive;
			EDetectorState previousState = EDetectorState::Inactive;
			bool stateChanged = false;
			bool hasNewData = false;

			DebugData() = default;
		};
#ifdef _MSC_VER
#pragma warning(pop)
#endif

	} // namespace SDK
} // namespace EWAcoustics
