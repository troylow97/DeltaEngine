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
  Vector2 interPoint;
  Vector2 DirectionVector;
  Vector2 ReflectionVector;
  Vector2 PointEnd;
  Vector2 normal;
  Vector2 center;
  Vector2 size;
  ColliderType type;
  float interTime;
  bool isCollideable;

  Collider() :
    interPoint { 0,0 },
    DirectionVector { 0,0 },
    ReflectionVector { 0,0 },
    PointEnd { 0,0 },
    normal { 0,0 },
    center { 0,0 },
    size { 0,0 },
    type { ColliderType::BOX },
    interTime { 0.0f },
    isCollideable { true }
  {}

  REGISTER_COMPONENT( Collider )
};

} // namespace DeltaEngine
