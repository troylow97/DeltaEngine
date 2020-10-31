#pragma once

#include "Core/Math/Vector.h"
#include "Components.h"
#include <rapidjson/prettywriter.h>
#include <rapidjson/filewritestream.h>"

namespace DeltaEngine
{
struct RigidBody
{
  Vector2 Direction;
  Vector2 Velocity;
  Vector2 AccumulatedForce;
  Vector2 Acceleration;
  Vector2 PointEnd;
  float Mass;
  float Movespeed;
  float Restitution;
  bool hasGravity;


  RigidBody() :
    Direction {0,0},
    Velocity { 0,0 },
    AccumulatedForce{0,0},
    Acceleration { 0,0 },
    PointEnd{0,0},
    Mass{1.0f},
    Movespeed{0.5f},
    Restitution{1.0f},
    hasGravity { false }
  {}

  REGISTER_COMPONENT( RigidBody )
};
}