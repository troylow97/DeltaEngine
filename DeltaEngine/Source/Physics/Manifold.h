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
  Vector2 Movement[2];
  float FrictionCof;

  Manifold::Manifold() :
    interTime { 0.0f },
    penetration { 0.0f }
  {}


};
}
