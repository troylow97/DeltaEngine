#pragma once
#include "Vector.h"
#include "DE_API.h"

namespace DeltaEngine
{
	struct DE_API Quaternion
	{
		float x, y, z, w;

		Quaternion();
		Quaternion(float, float, float, float);
		static Quaternion identity;
		static Quaternion AngleAxis(const float angle, const Vector3 axis);

		//assumes rotation sequencing of yaw, pitch, then roll, or Body 3-2-1
		static Quaternion EulerAnglesToQuaternion(Vector3 eulerAngles);

		static Quaternion EulerAnglesToQuaternion(float x, float y, float z);

		static Vector3 QuaternionToEulerAngles(Quaternion q);

	};
}
