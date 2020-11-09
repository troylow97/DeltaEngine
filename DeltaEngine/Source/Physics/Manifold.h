#pragma once
#include "Core/Math/Vector.h"
#include "Components/Collider.h"

namespace DeltaEngine
{
struct Manifold
{
  float interTime;
  float penetration;
  Vector2 normal;
  float FrictionCof{0.1f}; //not used for now

  Manifold::Manifold() :
    interTime { 0.0f },
    penetration { 0.0f }
  {}


};
}
