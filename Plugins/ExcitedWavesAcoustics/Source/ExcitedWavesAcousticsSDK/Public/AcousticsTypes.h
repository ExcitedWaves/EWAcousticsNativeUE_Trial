// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include <vector>
#include <utility>
#include <cstdint>

#if !defined(EWSDK_API)
#if defined(EWSDK_STATIC)
#define EWSDK_API
#elif defined(_MSC_VER)
#if defined(EWSDK_EXPORTS)
#define EWSDK_API __declspec(dllexport)
#else
#define EWSDK_API __declspec(dllimport)
#endif
#else
#define EWSDK_API
#endif
#endif

namespace EWAcoustics
{
	namespace SDK
	{

		using DetectorId = uint32_t;
		using RayId = uint32_t;
		using GroupId = uint8_t;

		struct Vector3;

		enum class ERayType : uint8_t
		{
			None,
			Side,
			Down,
			Up
		};

		enum class ERayScanningMethod : uint8_t
		{
			Umbrella,
			Uniform
		};

		struct EWSDK_API Vector3
		{
			float x = 0.0f, y = 0.0f, z = 0.0f;

			Vector3() = default;
			Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
		};

		struct EWSDK_API ProbeRequest
		{
			Vector3 origin;
			Vector3 direction;
			float maxDistance = 0.0f;
			RayId rayId = 0;
			RayId verificationTargetRayId = 0;
			ERayType rayType = ERayType::None;
			GroupId groupId = 0;
			bool requiresVerification = false;

			ProbeRequest() = default;

			ProbeRequest(const Vector3& origin_, const Vector3& direction_, float maxDistance_,
				RayId rayId_, ERayType rayType_, GroupId groupId_ = 0, bool requiresVerification_ = false, RayId verificationTargetRayId_ = 0)
				: origin(origin_), direction(direction_), maxDistance(maxDistance_), rayId(rayId_),
				verificationTargetRayId(verificationTargetRayId_), rayType(rayType_), groupId(groupId_), requiresVerification(requiresVerification_)
			{
			}
		};

		struct EWSDK_API ProbeResult
		{
			Vector3 hitPosition;
			Vector3 direction;
			Vector3 normal;
			Vector3 origin;
			float distance = 0.0f;
			float absorption = 0.0f;
			float groupSum = 0.0f;
			float outlierWeight = 1.0f;
			RayId rayId = 0;
			RayId verificationTargetRayId = 0;
			ERayType rayType = ERayType::None;
			GroupId groupId = 0;
			bool hit = false;
			bool requiresVerification = false;
			bool hasVerificationHit = false;
			bool isOutlier = false;

			ProbeResult() = default;
		};

		using ProbeRequests = std::vector<ProbeRequest>;
		using ProbeResults = std::vector<ProbeResult>;

		struct EWSDK_API AcousticsConfig
		{
			float maxAzimuthRayDistance = 10000.0f;
			float maxElevationRayDistance = 3000.0f;
			float maxProbeHeight = 100.0f;
			float ceilingElevationRaysOffset = 10.0f;
			float emitterElevationRaysOffset = 10.0f;
			float detectorCircleRadius = 150.0f;
			float verificationRayOffsetPercent = 10.0f;
			float angledRayBaseElevation = 30.0f;
			float angledRayElevationStep = 25.0f;
			float closeDistance = 500.0f;
			float midDistance = 1500.0f;
			float farDistance = 5000.0f;
			float positionChangeThreshold = 200.0f;
			float widthModulatorStrength = 3.0f;
			float heightBoostStrength = 0.5f;
			float binNoiseThreshold = 0.05f;
			float interpolationDuration = 1.0f;

			enum class EFrameLimitingMode : uint8_t
			{
				Global,
				PerDetector
			};

			ERayScanningMethod rayScanningMethod = ERayScanningMethod::Umbrella;
			EFrameLimitingMode frameLimitingMode = EFrameLimitingMode::Global;
			uint8_t pairsPerStep = 2;
			uint8_t azimuthSteps = 6;
			uint8_t elevationRays = 3;
			uint8_t uniformRaysCount = 30;
			uint8_t angledRayIterations = 2;
			uint8_t maxRaysPerFrame = 8;
			bool enableFrameLimiting = true;
			bool bypassOutliersDetection = false;
			bool bypassVerificationRays = false;
			bool useDynamicHeightSpread = true;
			bool enableApplyAbsorption = true;
			bool useResultsInterpolation = true;

			struct EWSDK_API ImageSourceConfig
			{
				float imageSourceRadius = 1500.0f;
				float positionMatchThreshold = 200.0f;
				float fadeInDuration = 0.15f;
				float fadeOutDuration = 0.3f;
				float reflectMaxDistance = 0.0f;
				uint32_t maxImageSources = 16;
				bool enableSpatialRadius = true;

				ImageSourceConfig() = default;
			};

			ImageSourceConfig imageSourceConfig;

			AcousticsConfig() = default;
		};

		struct EWSDK_API DistanceLODConfig
		{
			float lodDistanceRadius = 1000.0f;
			float closeEmitterThreshold = 50.0f;
			float farEmitterThreshold = 300.0f;
			bool enabled = false;

			DistanceLODConfig() = default;
		};

		struct EWSDK_API DistanceBinData
		{
			float close = 0.0f;
			float mid = 0.0f;
			float far = 0.0f;
			float outdoor = 0.0f;

			DistanceBinData() = default;
		};

		struct EWSDK_API AcousticsResults
		{
			DistanceBinData distanceBins;
			DistanceBinData absorptionBins;
			DistanceBinData absorptionMeans;
			float height = 0.0f;
			float width = 0.0f;
			float openness = 0.0f;
			float meanAbsorption = 0.0f;
			float presetAbsorption = 0.0f;
			float opennessCompensation = 0.0f;
			float reverbEnclosure = 0.0f;
			float sideRatio = 0.0f;
			float upRatio = 0.0f;
			float outdoorReverb = 0.0f;
			uint8_t upRays = 0;
			uint8_t verificationRays = 0;
			uint8_t upHits = 0;
			uint8_t sideRays = 0;
			uint8_t sideHits = 0;
			bool isIndoor = true;

			AcousticsResults() = default;
		};

		enum class ECurveShape : uint8_t
		{
			Linear,
			Smooth,
			Exponential,
			Inverse,
			Curve
		};

		struct EWSDK_API CurveTable
		{
			static constexpr int kMaxPoints = 64;
			float distances[kMaxPoints] = {};
			float values[kMaxPoints] = {};
			int count = 0;
		};

		struct EWSDK_API ListenerBlendConfig
		{
			CurveTable crossRoomBlendCurveTable;
			CurveTable distanceAttenuationCurveTable;

			float crossRoomBlendMaxDistance = 3000.0f;
			float crossRoomBlendMinDistance = 0.0f;
			float crossRoomBlendMinValue = 0.0f;
			float distanceAttenuationMaxDistance = 2000.0f;
			float distanceAttenuationMinDistance = 0.0f;
			float distanceAttenuationMinValue = 0.3f;
			float maxCrossRoomContribution = 0.3f;
			float maxSendLevel = 1.0f;
			float enclosureScaleMin = 0.3f;

			ECurveShape crossRoomBlendCurveShape = ECurveShape::Exponential;
			ECurveShape distanceAttenuationCurveShape = ECurveShape::Exponential;
			bool enableCrossRoomBlending = false;
			/** Late reverb send scale vs emitter-to-listener distance (0–1). Editor: Direct-To-Reverb Ratio. */
			bool enableDistanceAttenuation = false;
			bool enableEnclosureScaling = false;

			ListenerBlendConfig() = default;
		};

		struct EWSDK_API SendGroup
		{
			float closeRev = 0.0f;
			float midRev = 0.0f;
			float farRev = 0.0f;
			float outdoorRev = 0.0f;
			float meanAbsorption = 0.0f;
			float presetAbsorption = 0.0f;
			float closeAbsorption = 0.0f;
			float midAbsorption = 0.0f;
			float farAbsorption = 0.0f;
			float outdoorAbsorption = 0.0f;

			SendGroup() = default;
		};

		struct EWSDK_API ProcessedSendLevels
		{
			SendGroup emitter;
			SendGroup crossRoom;
			float distanceToListener = 0.0f;
			float crossRoomContribution = 0.0f;
			/** Late-reverb scale from direct-to-reverb ratio curve (1 if feature off or no listener). */
			float reverbMultiplier = 1.0f;
			bool hasListenerData = false;
			bool bHasValidResults = false;

			ProcessedSendLevels() = default;
		};

		struct EWSDK_API ImageSource
		{
			Vector3 position;
			Vector3 wallHitPoint;
			uint32_t sourceId = 0;
			uint32_t originalRayId = 0;
			float distance = 0.0f;
			float level = 1.0f;
			ERayType rayType = ERayType::None;

			ImageSource() = default;

			ImageSource(uint32_t id, const Vector3& pos, const Vector3& hitPt, uint32_t rayId, float dist, float lvl, ERayType type)
				: position(pos), wallHitPoint(hitPt), sourceId(id), originalRayId(rayId), distance(dist), level(lvl), rayType(type)
			{
			}
		};

		using ImageSources = std::vector<ImageSource>;

		enum class EDetectorState : uint8_t
		{
			Inactive,
			Scanning,
			Collecting,
			Processing
		};

	}
}
