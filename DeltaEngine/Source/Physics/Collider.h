#pragma once
#include "Core/Math/Vector.h"
#include "Core/Debugging/Gizmos.h"

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
      void OnDrawGizmos()
      {
          switch (type)
          {
          case ColliderType::BOX:
              Gizmos::Draw2DWireBox(center, size);
              break;
          case ColliderType::CIRCLE:
              Gizmos::Draw2DCircle(center, size);
              break;
          case ColliderType::RAY:
              Gizmos::Draw2DWireBox(center, size); //TO CHANGE
              break;
          case ColliderType::LINE:
              Gizmos::Draw2DWireBox(center, size); //TO CHANGE
              break;
          }
      }

  };

  ////ERROR: NEED TO FIX
  //void OnDrawGizmos(Collider& col)
  //{
  //    ColliderType type = col.type;
  //
  //    switch (type)
  //    {
  //    case ColliderType::BOX:
  //         Gizmos::Draw2DWireBox(col.center,col.size);
  //    case ColliderType::CIRCLE:
  //        Gizmos::Draw2DCircle(col.center, col.size);
  //    case ColliderType::RAY:
  //        Gizmos::Draw2DWireBox(col.center, col.size); //TO CHANGE
  //    case ColliderType::LINE:
  //        Gizmos::Draw2DWireBox(col.center, col.size); //TO CHANGE
  //    }
  //}
} // namespace DeltaEngine
