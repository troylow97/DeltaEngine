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
  void PhysicsSystem::UpdateComponents();

private:
    void PhysicsSystem::MoveInput();
  void PhysicsSystem::UpdateVelocity();

  Vector2 m_gravity_amount = { 0,-0.1};
  float m_max_velocity = 1000.0f;
  END_DEFINE_SYSTEM( PhysicsSystem )
}
