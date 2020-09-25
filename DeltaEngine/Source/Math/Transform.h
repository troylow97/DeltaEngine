#pragma once
#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"
#include "Core/Core.h"

namespace DeltaEngine
{
	class DE_API Transform
	{
	public:
		Transform* parent;
		Vector3 position;
		Quaternion rotation;
		Vector3 scale;
		Transform();
		Transform(Vector3 position, Quaternion rotation, Vector3 scale);
		Matrix4x4 LocalToWorldMatrix(bool withParents = true);
	};
}
