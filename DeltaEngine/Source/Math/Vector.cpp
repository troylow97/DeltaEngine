#include "DEpch.h"
#include "Vector.h"
#include <cmath>

Vector2::Vector2() : x{ 0 }, y{ 0 } {}
Vector2::Vector2(float v0, float v1) : x{ v0 }, y{ v1 } {}
Vector2 Vector2::zero { 0, 0 };
Vector2 Vector2::one { 1, 1 };
Vector2 Vector2::up { 0, 1 };
Vector2 Vector2::down { 0, -1 };
Vector2 Vector2::left { -1, 0 };
Vector2 Vector2::right { 1, 0 };
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
#pragma region Vector2 Operators
Vector2& Vector2::operator=(const Vector2& a)
{
	this->x = a.x;
	this->y = a.y;
	return *this;
}
Vector2 Vector2::operator+(const Vector2 a)
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
Vector2 Vector2::operator-(const Vector2 a)
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
Vector2 Vector2::operator-()
{
	Vector2 v;
	v.x = -this->x;
	v.y = -this->y;
	return v;
}
Vector2 Vector2::operator*(const Vector2 a)
{
	Vector2 v;
	v.x = this->x * a.x;
	v.y = this->y * a.y;
	return v;
}
Vector2 Vector2::operator*(const float f)
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
Vector2 Vector2::operator/(const float f)
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
Vector3 Vector3::zero { 0, 0 };
Vector3 Vector3::one { 1, 1, 1 };
Vector3 Vector3::up { 0, 1 };
Vector3 Vector3::down { 0, -1 };
Vector3 Vector3::left { -1, 0 };
Vector3 Vector3::right { 1, 0 };
Vector3 Vector3::forward { 0, 0, 1 };
Vector3 Vector3::back { 0, 0, -1 };
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
Vector3 Vector3::operator+(const Vector3 a)
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
Vector3 Vector3::operator-(const Vector3 a)
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
Vector3 Vector3::operator-()
{
	Vector3 v;
	v.x = -this->x;
	v.y = -this->y;
	v.z = -this->z;
	return v;
}
Vector3 Vector3::operator*(const Vector3 a)
{
	Vector3 v;
	v.x = this->x * a.x;
	v.y = this->y * a.y;
	v.z = this->z * a.z;
	return v;
}
Vector3 Vector3::operator*(const float f)
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
Vector3 Vector3::operator/(const float f)
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
