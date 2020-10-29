#pragma once

#include "Core/Math/Vector.h"
#include "Components.h"
#include <rapidjson/prettywriter.h>
#include <rapidjson/filewritestream.h>
#include  "Physics/Force.h"

namespace DeltaEngine
{
struct RigidBody
{
  Force Friction;
  Vector2 Direction;
  Vector2 Velocity;
  Vector2 ReflectedVector;
  Vector2 Acceleration;
  float Mass;
  float Movespeed;
  float inherentAcceleration;
  float Restitution; //not used
  bool hasGravity;


  RigidBody() :
    Velocity { 0,0 },
    Acceleration { 0,0 },
    inherentAcceleration { 3.0f },
    hasGravity { false }
  {}

  REGISTER_COMPONENT( RigidBody )
};
}