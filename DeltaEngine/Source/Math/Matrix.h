#pragma once

#include "Core/Core.h"
#include "Vector.h"
#include "Quaternion.h"

namespace DeltaEngine
{
#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif

	union DE_API Matrix4x4
	{
		struct
		{
			float m00, m01, m02, m03;
			float m10, m11, m12, m13;
			float m20, m21, m22, m23;
			float m30, m31, m32, m33;
		};

		float m[16];
		float m2[4][4];

		static Matrix4x4 identity;

		Matrix4x4();
		Matrix4x4(
			float, float, float, float,
			float, float, float, float,
			float, float, float, float,
			float, float, float, float);
		Matrix4x4(const float* arr);
		Matrix4x4(const Matrix4x4& copy);
		Matrix4x4& operator=(const Matrix4x4& rhs);

		Matrix4x4& operator *= (const Matrix4x4& rhs);

		static Matrix4x4 Rotate(const Quaternion q);
		static Matrix4x4 Rotate(const Vector3 v);
		static Matrix4x4 Scale(const Vector3 v);
		static Matrix4x4 Scale(const float f);
		static Matrix4x4 Translate(const Vector3 v);
		static Matrix4x4 Transpose(const Matrix4x4 m);

		static Matrix4x4 Ortho(
			const float left, const float right,
			const float bottom, const float top,
			const float zNear, const float zFar);
		static Matrix4x4 Perspective(
			const float fov, const float aspect,
			const float zNear, const float zFar);
	};

#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( default : 4201 )
#endif

	Matrix4x4 operator* (const Matrix4x4& lhs, const Matrix4x4& rhs);

	Vector3 operator* (const Matrix4x4& pMtx, const Vector3& rhs);
}
