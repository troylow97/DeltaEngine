#pragma once

#include "Core/Math/Vector.h"

namespace DeltaEngine
{
  struct RigidBody
  {
    Vector2 Direction;
    Vector2 Velocity;
    Vector2 ReflectedVector;
    Vector2 Acceleration;
    Vector2 AccumulatedForce;
    Vector2 PointEnd;
    float Mass;
    float Movespeed;
    float Restitution;
    float FrictionCoeff;
    float InherentAcceleration;
    float MaxAcceleration;
    float AccelerationPickup;
    bool hasGravity;
    bool isMoveable;

    RigidBody() :
      Direction{0, 0},
      Velocity{0, 0},
      ReflectedVector{0, 0},
      Acceleration{0, 0},
      AccumulatedForce{0, 0},
      PointEnd{0, 0},
      Mass{20.0f},
      Movespeed{10.0f},
      Restitution{1.0f},
      FrictionCoeff{0.85f},
      InherentAcceleration{0.0f},
      MaxAcceleration{40.0f},
      AccelerationPickup{3.0f},
      hasGravity{false},
      isMoveable{true}
    {
    }
  };
}
