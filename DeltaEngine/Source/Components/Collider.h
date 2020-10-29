#pragma once

#include "Core/Math/Vector.h"
#include "Components.h"

namespace DeltaEngine
{
enum class ColliderType
{
  NONE,
  BOX,
  CIRCLE,
  LINE,
  RAY
};

struct Collider
{
  Vector2 center;
  Vector2 size;
  Vector2 interPoint;
  ColliderType type;
  Vector2 collided_spot;
  bool isCollideable;
  bool isWall;
  Collider() :
    center { 0,0 },
    size { 0,0 },
    interPoint{0,0},
    type { ColliderType::BOX },
    collided_spot{0,0},
    isCollideable{ true },
    isWall{false}
  {}
  Collider( Vector2 new_center, Vector2 new_size, ColliderType col_type ) :
    center( new_center ),
    size( new_size ),
      interPoint{ 0,0 },
    type( col_type ),
    collided_spot{ 0,0 },
      isCollideable{ true }
  {}

  REGISTER_COMPONENT( Collider )
};

} // namespace DeltaEngine
