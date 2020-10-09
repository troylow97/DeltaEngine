#include "DEpch.h"
#include "Vector.h"
#include <cmath>

namespace DeltaEngine
{
	Vector2::Vector2() : x{ 0 }, y{ 0 } {}
	Vector2::Vector2(float v0, float v1) : x{ v0 }, y{ v1 } {}
	Vector2::Vector2(Vector3 v) : x{ v.x }, y{ v.y } {}
	const Vector2 Vector2::zero() { return { 0, 0 }; }
	const Vector2 Vector2::one() { return { 1, 1 }; }
	const Vector2 Vector2::up() { return { 0, 1 }; }
	const Vector2 Vector2::down() { return { 0, -1 }; }
	const Vector2 Vector2::left() { return { -1, 0 }; }
	const Vector2 Vector2::right() { return { 1, 0 }; }
	Vector2 Vector2::Normalize()
	{
		if (SqrMagnitude() < (1 << 8))
			return *this;
		return *this /= Magnitude();
	}
	Vector2 Vector2::normalized() const
	{
		if (SqrMagnitude() < (1 << 8))
			return *this;
		Vector2 v{ x, y };
		return v /= Magnitude();
	}
	float Vector2::SqrMagnitude() const
	{
		return x * x + y * y;
	}
	float Vector2::Magnitude() const
	{
		return sqrtf(SqrMagnitude());
	}
	float Vector2::DotProduct(const Vector2& a) const
	{
		return x + a.x + y + a.y;
	}

	Vector2 Vector2::CrossProduct_Vector(const Vector2& a) const
	{
		return Vector2{ a.y,-a.x };
	}

	float Vector2::CrossProduct_Magnitude(const Vector2& a) const
	{
		return (x * a.y) - (y * a.x);
	}

#pragma region Vector2 Operators
	Vector2& Vector2::operator=(const Vector2& a)
	{
		x = a.x;
		y = a.y;
		return *this;
	}
	Vector2 Vector2::operator+(const Vector2 a) const
	{
		Vector2 v;
		v.x = x + a.x;
		v.y = y + a.y;
		return v;
	}
	Vector2 Vector2::operator+(const float f)
	{
		x += f;
		y += f;
		return *this;
	}
	Vector2 Vector2::operator+=(const Vector2 a)
	{
		x += a.x;
		y += a.y;
		return *this;
	}
	Vector2 Vector2::operator-(const Vector2 a) const
	{
		Vector2 v;
		v.x = x - a.x;
		v.y = y - a.y;
		return v;
	}
	Vector2 Vector2::operator-=(const Vector2 a)
	{
		x -= a.x;
		y -= a.y;
		return *this;
	}
	Vector2 Vector2::operator-() const
	{
		Vector2 v;
		v.x = -x;
		v.y = -y;
		return v;
	}
	Vector2 Vector2::operator-(const float f)
	{
		x -= f;
		y -= f;
		return *this;
	}
	Vector2 Vector2::operator*(const Vector2 a) const
	{
		Vector2 v;
		v.x = x * a.x;
		v.y = y * a.y;
		return v;
	}
	Vector2 Vector2::operator*(const float f) const
	{
		Vector2 v;
		v.x = x * f;
		v.y = y * f;
		return v;
	}

	Vector2 Vector2::operator*=(const Vector2 a)
	{
		x *= a.x;
		y *= a.y;
		return *this;
	}
	Vector2 Vector2::operator*=(const float f)
	{
		x *= f;
		y *= f;
		return *this;
	}
	Vector2 Vector2::operator/(const float f) const
	{
		Vector2 v;
		v.x = x / f;
		v.y = y / f;
		return v;
	}
	Vector2 Vector2::operator/=(const float f)
	{
		x /= f;
		y /= f;
		return *this;
	}
	bool Vector2::operator!=(const Vector2 v) const
	{
		if (x == v.x && y == v.y)
			return false;

		return true;
	}
	bool Vector2::operator==(const Vector2 v) const
	{
		if (x == v.x && y == v.y)
			return true;

		return false;
	}

#pragma endregion

	Vector3::Vector3() : x{ 0 }, y{ 0 }, z{ 0 } {}
	Vector3::Vector3(float v0, float v1) : x{ v0 }, y{ v1 }, z{ 0 } {}
	Vector3::Vector3(float v0, float v1, float v2) : x{ v0 }, y{ v1 }, z{ v2 } {}
	Vector3::Vector3(Vector2 v) : x{ v.x }, y{ v.y }, z{ 0 } {}
	const Vector3 Vector3::zero() { return { 0, 0 }; }
	const Vector3 Vector3::one() { return { 1, 1, 1 }; }
	const Vector3 Vector3::up() { return { 0, 1 }; }
	const Vector3 Vector3::down() { return { 0, -1 }; }
	const Vector3 Vector3::left() { return { -1, 0 }; }
	const Vector3 Vector3::right() { return { 1, 0 }; }
	const Vector3 Vector3::forward() { return { 0, 0, 1 }; }
	const Vector3 Vector3::back() { return { 0, 0, -1 }; }
	Vector3 Vector3::Normalize()
	{
		if (SqrMagnitude() < (1 << 8))
			return *this;
		return *this /= Magnitude();
	}
	Vector3 Vector3::normalized() const
	{
		if (SqrMagnitude() < (1 << 8))
			return *this;
		Vector3 v{ x, y, z };
		return v /= Magnitude();
	}
	float Vector3::SqrMagnitude() const
	{
		return x * x + y * y + z * z;
	}
	float Vector3::Magnitude() const
	{
		return sqrtf(SqrMagnitude());
	}
#pragma region Vector3 Operators
	Vector3& Vector3::operator=(const Vector3& a)
	{
		x = a.x;
		y = a.y;
		z = a.z;
		return *this;
	}
	Vector3& Vector3::operator=(const Vector2& a)
	{
		x = a.x;
		y = a.y;
		z = 0;
		return *this;
	}
	Vector3 Vector3::operator+(const Vector3 a) const
	{
		Vector3 v;
		v.x = x + a.x;
		v.y = y + a.y;
		v.z = z + a.z;
		return v;
	}
	Vector3 Vector3::operator+=(const Vector3 a)
	{
		x += a.x;
		y += a.y;
		z += a.z;
		return *this;
	}
	Vector3 Vector3::operator-(const Vector3 a) const
	{
		Vector3 v;
		v.x = x - a.x;
		v.y = y - a.y;
		v.z = z - a.z;
		return v;
	}
	Vector3 Vector3::operator-=(const Vector3 a)
	{
		x -= a.x;
		y -= a.y;
		z -= a.z;
		return *this;
	}
	Vector3 Vector3::operator-() const
	{
		Vector3 v;
		v.x = -x;
		v.y = -y;
		v.z = -z;
		return v;
	}
	Vector3 Vector3::operator*(const Vector3 a) const
	{
		Vector3 v;
		v.x = x * a.x;
		v.y = y * a.y;
		v.z = z * a.z;
		return v;
	}
	Vector3 Vector3::operator*(const float f) const
	{
		Vector3 v;
		v.x = x * f;
		v.y = y * f;
		v.z = z * f;
		return v;
	}
	Vector3 Vector3::operator*=(const Vector3 a)
	{
		x *= a.x;
		y *= a.y;
		z *= a.z;
		return *this;
	}
	Vector3 Vector3::operator*=(const float f)
	{
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}
	Vector3 Vector3::operator/(const float f) const
	{
		Vector3 v;
		v.x = x / f;
		v.y = y / f;
		v.z = z / f;
		return v;
	}
	Vector3 Vector3::operator/=(const float f)
	{
		x /= f;
		y /= f;
		z /= f;
		return *this;
	}
#pragma endregion


	Vector4::Vector4() : x{ 0 }, y{ 0 }, z{ 0 }, w{ 0 } {}
	Vector4::Vector4(float v0, float v1, float v2) : x{ v0 }, y{ v1 }, z{ v2 }, w{ 0 } {}
	Vector4::Vector4(float v0, float v1, float v2, float v3) : x{ v0 }, y{ v1 }, z{ v2 }, w{ v3 } {}

	float Vector2DotProduct(const Vector2& pVec0, const Vector2& pVec1)
	{
		return pVec0.x * pVec1.x + pVec0.y * pVec1.y;
	}

	float Vector2Length(const Vector2& pVec0)
	{
		return sqrt(pVec0.x * pVec0.x + pVec0.y * pVec0.y);
	}

	Vector2 Normalise(Vector2& v)
	{
		float magnitude = Vector2Length(v);
		Vector2 pResult{ 0,0 };

		if (magnitude != 1 && magnitude != 0)
		{
			pResult.x = v.x / magnitude;
			pResult.y = v.y / magnitude;
		}

		return pResult;
	}

}
