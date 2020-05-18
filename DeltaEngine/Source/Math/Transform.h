#pragma once
#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"

namespace DeltaEngine
{
	class Transform
	{
	public:
		Vector3 position;
		Vector3 eulerAngles;
		Vector3 scale;
		Quaternion rotation;
		Transform();
		Matrix4x4 LocalToWorldMatrix();
	};
}
