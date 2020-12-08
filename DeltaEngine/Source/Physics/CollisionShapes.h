/**********************************************************************************
* \file   CollisionShapes.h
* \brief  The file contains BLAHBLAHBLAH
* \author Chin, Clara,   X% Code Contribution
* \author Low, Troy,     X% Code Contribution
* \author Ong, Graeme,   X% Code Contribution
* \author Tan, Tong Wee, X% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "DEpch.h"
#include "Core/Math/Vector.h"

namespace DeltaEngine
{
  struct AABB
  {
  public:
    Vector2 min;
    Vector2 max;
    AABB();
    AABB(Vector2 center, Vector2 size);
    void CalculatePosition(Vector2 center, Vector2 size);
    void Combine(const AABB& aabb);
    void Combine(const AABB& aabb1, const AABB& aabb2);
    bool Contains(const AABB& aabb) const;
    float GetPerimeter() const;
  };

  struct LineSegment
  {
    LineSegment();
    LineSegment(Vector2 pt0, Vector2 pt1);
    LineSegment(const Vector2& pos, const Vector2& scale, const Vector2& dir);
    Vector2 m_pt0;
    Vector2 m_pt1;
    Vector2 m_normal; //outward
  };

  struct Circle //Does not support circle with non equal radius
  {
    Circle();
    Circle(Vector2 center, float radius, float mass);
    Circle(Vector2 center, float radius);
    Circle(Vector2 center, Vector2 radius);
    Vector2 m_center;
    float m_radius;
    float m_mass;
  };

  struct Ray
  {
    Ray();
    Ray(Vector2 pt, Vector2 dir);
    Vector2 m_pt0;
    Vector2 m_dir;
  };
}
