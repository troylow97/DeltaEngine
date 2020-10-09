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
      Vector2 center;
      Vector2 size;
      ColliderType type;
      Vector2 interPoint;
      Collider() :
          isCollideable{true},
          center{0,0},
          size{0,0},
          type{ColliderType::BOX}
      {}
      Collider(Vector2 new_center,Vector2 new_size,ColliderType col_type) :
          center(new_center),
          size(new_size),
          type(col_type)
      {}
  };

} // namespace DeltaEngine
