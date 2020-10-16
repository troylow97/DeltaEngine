#pragma once

#include "Core/Math/Vector.h"
#include "Components.h"
#include <rapidjson/prettywriter.h>
#include <rapidjson/filewritestream.h>

namespace DeltaEngine
{
struct RigidBody
{
  Vector2 Direction;
  Vector2 Velocity;
  Vector2 ReflectedVector;
  Vector2 Acceleration;
  float Mass;
  float Friction;
  float Movespeed;
  float inherentAcceleration;
  float Restituition; //not used
  bool hasGravity;
  bool isMoveable;


  RigidBody() :
    Velocity { 0,0 },
    Acceleration { 0,0 },
    Friction { 0.97f },
    inherentAcceleration { 3.0f },
    hasGravity { false },
    isMoveable { true }
  {}

  REGISTER_COMPONENT( RigidBody )
};
}