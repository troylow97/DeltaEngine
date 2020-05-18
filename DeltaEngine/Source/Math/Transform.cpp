#include "DEpch.h"
#include "Transform.h"

namespace DeltaEngine
{
	Transform::Transform() : position{}, rotation{}, eulerAngles{}, scale{ 1, 1, 1 } {}

	Matrix4x4 Transform::LocalToWorldMatrix()
	{
		return Matrix4x4::Transpose(Matrix4x4::Translate(position)) * Matrix4x4::Scale(scale) * Matrix4x4::Rotate(rotation);
	}
}
