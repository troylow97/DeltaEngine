#pragma once

struct Vector2
{
	float x, y;
	Vector2();
	Vector2(float, float);
	static Vector2 zero, one;
	static Vector2 up, down, left, right;
	Vector2 Normalize();
	Vector2 normalized() const;
	float Magnitude() const;
	float SqrMagnitude() const;
#pragma region Vector2 Operators
	Vector2& operator=(const Vector2& a);
	Vector2 operator+(const Vector2 a);
	Vector2 operator+=(const Vector2 a);
	Vector2 operator-(const Vector2 a);
	Vector2 operator-=(const Vector2 a);
	Vector2 operator-();
	Vector2 operator*(const Vector2 f);
	Vector2 operator*(const float f);
	Vector2 operator*=(const Vector2 f);
	Vector2 operator*=(const float f);
	Vector2 operator/(const float f);
	Vector2 operator/=(const float f);
#pragma endregion
};
struct Vector3
{
	float x, y, z;
	Vector3();
	Vector3(float, float);
	Vector3(float, float, float);
	static Vector3 zero, one;
	static Vector3 up, down, left, right, forward, back;
	Vector3 Normalize();
	Vector3 normalized() const;
	float Magnitude() const;
	float SqrMagnitude() const;
#pragma region Vector3 Operators
	Vector3& operator=(const Vector3& a);
	Vector3& operator=(const Vector2& a);
	Vector3 operator+(const Vector3 a);
	Vector3 operator+=(const Vector3 a);
	Vector3 operator-(const Vector3 a);
	Vector3 operator-=(const Vector3 a);
	Vector3 operator-();
	Vector3 operator*(const Vector3 f);
	Vector3 operator*(const float f);
	Vector3 operator*=(const Vector3 f);
	Vector3 operator*=(const float f);
	Vector3 operator/(const float f);
	Vector3 operator/=(const float f);
#pragma endregion
};
struct Vector4
{
	float x, y, z, w;
	Vector4();
	Vector4(float, float, float);
	Vector4(float, float, float, float);
};
