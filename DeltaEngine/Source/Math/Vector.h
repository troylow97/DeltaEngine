#pragma once

#include "Core/Core.h"

namespace DeltaEngine
{
	struct DE_API Vector2
	{
		float x, y;
		Vector2();
		Vector2(float, float);
		static const Vector2 zero();
		static const Vector2 one();
		static const Vector2 up();
		static const Vector2 down();
		static const Vector2 left();
		static const Vector2 right();
		Vector2 Normalize();
		Vector2 normalized() const;
		float Magnitude() const;
		float SqrMagnitude() const;
		float DotProduct(const Vector2& a) const;
		Vector2 CrossProduct_Vector(const Vector2& a) const; //returns a vector perpendicular to the input vector still in the same 2D plane.
		float CrossProduct_Magnitude(const Vector2& a) const;
#pragma region Vector2 Operators
		Vector2& operator=(const Vector2& a);
		Vector2 operator+(const Vector2 a) const;
		Vector2 operator+=(const Vector2 a);
		Vector2 operator-(const Vector2 a) const;
		Vector2 operator-=(const Vector2 a);
		Vector2 operator-() const;
		Vector2 operator*(const Vector2 f) const;
		Vector2 operator*(const float f) const;
		Vector2 operator*=(const Vector2 f);
		Vector2 operator*=(const float f);
		Vector2 operator/(const float f) const;
		Vector2 operator/=(const float f);
#pragma endregion
	};
	struct DE_API Vector3
	{
		float x, y, z;
		Vector3();
		Vector3(float, float);
		Vector3(float, float, float);
		static const Vector3 zero();
		static const Vector3 one();
		static const Vector3 up();
		static const Vector3 down();
		static const Vector3 left();
		static const Vector3 right();
		static const Vector3 forward();
		static const Vector3 back();
		Vector3 Normalize();
		Vector3 normalized() const;
		float Magnitude() const;
		float SqrMagnitude() const;
#pragma region Vector3 Operators
		Vector3& operator=(const Vector3& a);
		Vector3& operator=(const Vector2& a);
		Vector3 operator+(const Vector3 a) const;
		Vector3 operator+=(const Vector3 a);
		Vector3 operator-(const Vector3 a) const;
		Vector3 operator-=(const Vector3 a);
		Vector3 operator-() const;
		Vector3 operator*(const Vector3 f) const;
		Vector3 operator*(const float f) const;
		Vector3 operator*=(const Vector3 f);
		Vector3 operator*=(const float f);
		Vector3 operator/(const float f) const;
		Vector3 operator/=(const float f);
#pragma endregion
	};
	struct DE_API Vector4
	{
		float x, y, z, w;
		Vector4();
		Vector4(float, float, float);
		Vector4(float, float, float, float);
	};
}
