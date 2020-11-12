#pragma once

#include "Core/Math/Vector.h"

namespace DeltaEngine
{
  enum class ColliderType : unsigned
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
    bool isCollideable;
    bool isTrigger;
    bool isCollidingOnFloor;
    char CollisionLayerID;
    char CollisionLayerCheck;

    Collider() :
      center{0, 0},
      size{0, 0},
      interPoint{0, 0},
      type{ColliderType::BOX},
      isCollideable{true},
      isTrigger{false},
      isCollidingOnFloor{false}
    {
    }

    Collider(Vector2 new_center, Vector2 new_size, ColliderType col_type) :
      center(new_center),
      size(new_size),
      interPoint{0, 0},
      type{ColliderType::BOX},
      isCollideable{true},
      isTrigger{false},
      isCollidingOnFloor{false}
    {
      (void) col_type;
    }
  };
} // namespace DeltaEngine
