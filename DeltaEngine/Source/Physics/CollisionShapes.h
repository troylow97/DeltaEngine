#pragma once
#include "DEpch.h"
#include "Core/Math/Vector.h"

namespace DeltaEngine
{
struct DE_API AABB
{
public:
  Vector2 min;
  Vector2 max;
  AABB();
  AABB( Vector2 center, Vector2 size );
  void CalculatePosition( Vector2 center, Vector2 size );
  void Combine( const AABB &aabb );
  void Combine( const AABB &aabb1, const AABB &aabb2 );
  bool Contains( const AABB &aabb ) const;
  float GetPerimeter() const;
};

struct DE_API LineSegment
{
  LineSegment();
  LineSegment( const Vector2 pt0, const Vector2 pt1 );
  LineSegment( const Vector2 &pos, const Vector2 &scale, const Vector2 &dir );
  Vector2	m_pt0;
  Vector2	m_pt1;
  Vector2	m_normal; //outward
};

struct DE_API Circle //Does not support circle with non equal radius
{
  Circle();
  Circle( const Vector2 center, const float radius, const float mass );
  Circle( const Vector2 center, const float radius );
  Circle( const Vector2 center, const Vector2 radius );
  Vector2  m_center;
  float	m_radius;
  float   m_mass;
};

struct DE_API Ray
{
  Ray();
  Ray( const Vector2 pt, const Vector2 dir );
  Vector2	m_pt0;
  Vector2	m_dir;
};
}