#pragma once
#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"
#include "DE_API.h"

namespace DeltaEngine
{
	class DE_API Transform
	{
	public:
		Transform* parent;
		Vector3 position;
		Vector3 scale;
		Quaternion rotation;
		Transform();
		Matrix4x4 LocalToWorldMatrix(bool withParents = true);
	};
}
