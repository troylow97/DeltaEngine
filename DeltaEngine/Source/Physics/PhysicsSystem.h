#pragma once
#include "Core/Math/Vector.h"
#include "CollisionSystem.h"
#include "Components/Collider.h"
#include "ECS/ECSModule.h"

namespace DeltaEngine
{
DEFINE_SYSTEM( PhysicsSystem, RigidBody, Transform, Collider )
public:
  PhysicsSystem::PhysicsSystem() = default;
  ~PhysicsSystem() = default;

  void PhysicsSystem::Initialize();
  virtual void PhysicsSystem::Update() override;
  virtual void PhysicsSystem::LateUpdate() override;

private:
  void PhysicsSystem::UpdateVelocity();

  Vector2 m_gravity_amount = { 0,-12.0f };
  float JumpForce;
  float InitialJumpForce;
  int CurrentJumpTicks;
  int MaxJumpTicks;
  float m_max_velocity;
  END_DEFINE_SYSTEM( PhysicsSystem )
}
