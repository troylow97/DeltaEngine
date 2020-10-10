#pragma once
#include "Core/Math/DE_Math.h"
#include "Components.h"

namespace DeltaEngine
{
	struct DE_API Transform
	{
	public:
		Vector3 old_position;
		Vector3 position;
		Vector3 scale;
		Quaternion rotation;
		Transform();
		Transform(Vector3 position, Quaternion rotation, Vector3 scale);
		Matrix4x4 LocalToWorldMatrix();

		REGISTER_COMPONENT(Transform)
	};

}