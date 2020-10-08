#include "DEpch.h"
#include "Transform.h"

namespace DeltaEngine
{
	Transform::Transform() : parent{nullptr}, position{}, rotation{}, scale{ 1, 1, 1 } {}
	Transform::Transform(Vector3 pos, Quaternion rot, Vector3 sca)
		: parent{ nullptr }, position{ pos }, rotation{ rot }, scale{ sca } {}

	Matrix4x4 Transform::LocalToWorldMatrix(bool withParents)
	{
		Vector3 pos = position;
		Quaternion rot = rotation;
		Vector3 sca = scale;

		if (withParents)
		{
			Transform* pTrans = parent;

			while (pTrans)
			{
				pos += pTrans->position;
				rot *= pTrans->rotation;
				sca *= pTrans->scale;
				pTrans = pTrans->parent;
			}
		}

		return Matrix4x4::Scale(sca) * Matrix4x4::Rotate(rot) * Matrix4x4::Transpose(Matrix4x4::Translate(pos));
	}
}
