// Copyright 2025-2026 Excited Waves. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "ExcitedWavesAcousticsSDK/Public/AcousticsTypes.h"

namespace EWAcoustics
{
namespace SDK
{
	/**
	 * UE-specific conversion helpers between FVector and Vector3
	 */
	class EngineConversions
	{
	public:
		static Vector3 FromUEVector(const FVector& ueVector)
		{
			return Vector3(ueVector.X, ueVector.Y, ueVector.Z);
		}

		static FVector ToUEVector(const Vector3& vector)
		{
			return FVector(vector.x, vector.y, vector.z);
		}
	};
}
}
