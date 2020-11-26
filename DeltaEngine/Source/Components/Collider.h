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
    Vector2 offset;
    Vector2 size;
    Vector2 interPoint;
    ColliderType type;
    bool isTrigger;
    bool isCollidingOnFloor;
    int CollisionLayerCheck;
    int CollisionLayerID;

    Collider() :
      center{0, 0},
      offset{0,0},
      size{0, 0},
      interPoint{0, 0},
      type{ColliderType::BOX},
      isTrigger{false},
      isCollidingOnFloor{false},
      CollisionLayerCheck{255},
      CollisionLayerID{255}
    { }

    Collider(Vector2 new_center, Vector2 new_size, ColliderType col_type) :
      center(new_center),
      offset{0,0},
      size(new_size),
      interPoint{0, 0},
      type{ColliderType::BOX},
      isTrigger{false},
      isCollidingOnFloor{false},
      CollisionLayerCheck{255},
      CollisionLayerID{ 255 }
    {
      (void)col_type;
    }
  };

  //0001 Wall
  //0010 Player
  //0100 Enemy
  //1000 Bullet

} // namespace DeltaEngine
