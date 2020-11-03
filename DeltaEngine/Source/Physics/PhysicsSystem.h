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
  //CollisionSystem collision_system();

  virtual void PhysicsSystem::Update() override;
  virtual void PhysicsSystem::LateUpdate() override;

private:
  void PhysicsSystem::UpdateVelocity();

  Vector2 m_gravity_amount = { 0,-5.0f };
  float m_max_velocity = 1000.0f;
  END_DEFINE_SYSTEM( PhysicsSystem )
}
