#include "DEpch.h"
#include "Vector.h"
#include <cmath>

namespace DeltaEngine
{
	Vector2::Vector2() : x{ 0 }, y{ 0 } {}
	Vector2::Vector2(float v0, float v1) : x{ v0 }, y{ v1 } {}
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
		return this->x + a.x + this->y + a.y;
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
		this->x = a.x;
		this->y = a.y;
		return *this;
	}
	Vector2 Vector2::operator+(const Vector2 a) const
	{
		Vector2 v;
		v.x = this->x + a.x;
		v.y = this->y + a.y;
		return v;
	}
	Vector2 Vector2::operator+=(const Vector2 a)
	{
		this->x += a.x;
		this->y += a.y;
		return *this;
	}
	Vector2 Vector2::operator-(const Vector2 a) const
	{
		Vector2 v;
		v.x = this->x - a.x;
		v.y = this->y - a.y;
		return v;
	}
	Vector2 Vector2::operator-=(const Vector2 a)
	{
		this->x -= a.x;
		this->y -= a.y;
		return *this;
	}
	Vector2 Vector2::operator-() const
	{
		Vector2 v;
		v.x = -this->x;
		v.y = -this->y;
		return v;
	}
	Vector2 Vector2::operator*(const Vector2 a) const
	{
		Vector2 v;
		v.x = this->x * a.x;
		v.y = this->y * a.y;
		return v;
	}
	Vector2 Vector2::operator*(const float f) const
	{
		Vector2 v;
		v.x = this->x * f;
		v.y = this->y * f;
		return v;
	}
	Vector2 Vector2::operator*=(const Vector2 a)
	{
		this->x *= a.x;
		this->y *= a.y;
		return *this;
	}
	Vector2 Vector2::operator*=(const float f)
	{
		this->x *= f;
		this->y *= f;
		return *this;
	}
	Vector2 Vector2::operator/(const float f) const
	{
		Vector2 v;
		v.x = this->x / f;
		v.y = this->y / f;
		return v;
	}
	Vector2 Vector2::operator/=(const float f)
	{
		this->x /= f;
		this->y /= f;
		return *this;
	}
#pragma endregion

	Vector3::Vector3() : x{ 0 }, y{ 0 }, z{ 0 } {}
	Vector3::Vector3(float v0, float v1) : x{ v0 }, y{ v1 }, z{ 0 } {}
	Vector3::Vector3(float v0, float v1, float v2) : x{ v0 }, y{ v1 }, z{ v2 } {}
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
		this->x = a.x;
		this->y = a.y;
		this->z = a.z;
		return *this;
	}
	Vector3& Vector3::operator=(const Vector2& a)
	{
		this->x = a.x;
		this->y = a.y;
		this->z = 0;
		return *this;
	}
	Vector3 Vector3::operator+(const Vector3 a) const
	{
		Vector3 v;
		v.x = this->x + a.x;
		v.y = this->y + a.y;
		v.z = this->z + a.z;
		return v;
	}
	Vector3 Vector3::operator+=(const Vector3 a)
	{
		this->x += a.x;
		this->y += a.y;
		this->z += a.z;
		return *this;
	}
	Vector3 Vector3::operator-(const Vector3 a) const
	{
		Vector3 v;
		v.x = this->x - a.x;
		v.y = this->y - a.y;
		v.z = this->z - a.z;
		return v;
	}
	Vector3 Vector3::operator-=(const Vector3 a)
	{
		this->x -= a.x;
		this->y -= a.y;
		this->z -= a.z;
		return *this;
	}
	Vector3 Vector3::operator-() const
	{
		Vector3 v;
		v.x = -this->x;
		v.y = -this->y;
		v.z = -this->z;
		return v;
	}
	Vector3 Vector3::operator*(const Vector3 a) const
	{
		Vector3 v;
		v.x = this->x * a.x;
		v.y = this->y * a.y;
		v.z = this->z * a.z;
		return v;
	}
	Vector3 Vector3::operator*(const float f) const
	{
		Vector3 v;
		v.x = this->x * f;
		v.y = this->y * f;
		v.z = this->z * f;
		return v;
	}
	Vector3 Vector3::operator*=(const Vector3 a)
	{
		this->x *= a.x;
		this->y *= a.y;
		this->z *= a.z;
		return *this;
	}
	Vector3 Vector3::operator*=(const float f)
	{
		this->x *= f;
		this->y *= f;
		this->z *= f;
		return *this;
	}
	Vector3 Vector3::operator/(const float f) const
	{
		Vector3 v;
		v.x = this->x / f;
		v.y = this->y / f;
		v.z = this->z / f;
		return v;
	}
	Vector3 Vector3::operator/=(const float f)
	{
		this->x /= f;
		this->y /= f;
		this->z /= f;
		return *this;
	}
#pragma endregion


	Vector4::Vector4() : x{ 0 }, y{ 0 }, z{ 0 }, w{ 0 } {}
	Vector4::Vector4(float v0, float v1, float v2) : x{ v0 }, y{ v1 }, z{ v2 }, w{ 0 } {}
	Vector4::Vector4(float v0, float v1, float v2, float v3) : x{ v0 }, y{ v1 }, z{ v2 }, w{ v3 } {}
}
