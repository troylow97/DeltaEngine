/**********************************************************************************
* \file   Vector.h
* \brief  The file contains the class for Vector2 and Vector3
* \author Low, Troy,     20% Code Contribution
* \author Ong, Graeme,   80% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

namespace DeltaEngine
{
  struct Vector3;

  struct Vector2
  {
    float x, y;
    Vector2();
    Vector2(float, float);
    Vector2(Vector3);
    static const Vector2 zero();
    static const Vector2 one();
    static const Vector2 up();
    static const Vector2 down();
    static const Vector2 left();
    static const Vector2 right();
    Vector2 Normalize();
    float Length() const;
    Vector2 normalized() const;
    float Magnitude() const;
    float SqrMagnitude() const;
    float DotProduct(const Vector2& a) const;
    float CrossProduct_Magnitude(const Vector2& a) const;
    Vector2 CrossProduct_Vector(const Vector2& a) const;
    Vector2 Square();
    static float Distance(Vector2 a, Vector2 b);
    static Vector2 Lerp(Vector2 a, Vector2 b, float t);
#pragma region Vector2 Operators
    Vector2& operator=(const Vector2& a);
    Vector2 operator+(Vector2 a) const;
    Vector2 operator+(float f);
    Vector2 operator+=(Vector2 a);
    Vector2 operator-(Vector2 a) const;
    Vector2 operator-=(Vector2 a);
    Vector2 operator-() const;
    Vector2 operator-(float f);
    Vector2 operator*(Vector2 f) const;
    Vector2 operator*(float f) const;
    Vector2 operator*=(Vector2 f);
    Vector2 operator*=(float f);
    Vector2 operator/(float f) const;
    Vector2 operator/=(float f);
    bool operator>(Vector2 v) const;
    bool operator>(float f) const;
    bool operator==(Vector2 v) const;
    bool operator!=(Vector2 v) const;
#pragma endregion
  };

  struct Vector3
  {
    float x, y, z;
    Vector3();
    Vector3(float, float);
    Vector3(float, float, float);
    Vector3(Vector2);
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
    static Vector3 MoveTowards(Vector3 from, Vector3 to, float speed);
    static Vector3 Lerp(Vector3 a, Vector3 b, float t);
#pragma region Vector3 Operators
    Vector3& operator=(const Vector3& a);
    Vector3& operator=(const Vector2& a);
    Vector3 operator+(Vector3 a) const;
    Vector3 operator+=(Vector3 a);
    Vector3 operator-(Vector3 a) const;
    Vector3 operator-=(Vector3 a);
    Vector3 operator-() const;
    Vector3 operator*(Vector3 f) const;
    Vector3 operator*(float f) const;
    Vector3 operator*=(Vector3 f);
    Vector3 operator*=(float f);
    Vector3 operator/(float f) const;
    Vector3 operator/=(float f);
#pragma endregion
  };

  struct Vector4
  {
    float x, y, z, w;
    Vector4();
    Vector4(float, float, float);
    Vector4(float, float, float, float);
  };

  Vector2 operator*(float f, Vector2 v);
  float Vector2DotProduct(const Vector2& pVec0, const Vector2& pVec1);
  float Vector2Length(const Vector2& pVec0);
  Vector2 Normalise(Vector2& v);
}
