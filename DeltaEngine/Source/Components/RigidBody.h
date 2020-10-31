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
  float Restitution; //not used
  bool hasGravity;
  bool isMoveable;


  RigidBody() :
    Direction{0,0},
    Velocity { 0,0 },
    ReflectedVector{0,0},
    Acceleration { 0,0 },
    AccumulatedForce{0,0},
    PointEnd{0,0},
    Mass{1.0f},
    Movespeed{0.1f},
    Restitution{1.0f},
    hasGravity { false },
    isMoveable { true }
  {}

};
}