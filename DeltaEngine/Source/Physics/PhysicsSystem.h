#pragma once
#include "Core/Math/Vector.h"
#include "CollisionSystem.h"
#include "Components/Collider.h"
#include "ECS/ECSModule.h"

namespace DeltaEngine
{
  DEFINE_SYSTEM(PhysicsSystem, RigidBody, Transform, Collider)
  public:
    PhysicsSystem::PhysicsSystem() = default;
    ~PhysicsSystem() = default;

    void PhysicsSystem::Initialize() override;
    void PhysicsSystem::Update() override;
    void PhysicsSystem::LateUpdate() override;

  private:
    void PhysicsSystem::SetBounds(RigidBody& r1);
    void PhysicsSystem::UpdateVelocity();

    Vector2 m_gravity_amount {0, -60.0f};
    float JumpForce{4500.0f};
    float InitialJumpForce{4500.0f};
    int CurrentJumpTicks{0};
    int MaxJumpTicks{10};
    int CurrentDashTicks{ 0 };
    int MaxDashTicks{ 10 };
  END_DEFINE_SYSTEM(PhysicsSystem)
}
