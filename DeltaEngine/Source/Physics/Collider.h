#pragma once
#include "Core/Math/Vector.h"

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
      bool isCollideable;
      float interTime;
      Vector2 interPoint;
      Vector2 DirectionVector;
      Vector2 ReflectionVector;
      Vector2 PointEnd;
      Vector2 normal;
      Vector2 center;
      Vector2 size;
      ColliderType type;
      Collider() :
          isCollideable{true},
          interTime{0.0f},
          center{0,0},
          size{0,0},
          type{ColliderType::BOX}
      {}
  };

} // namespace DeltaEngine
