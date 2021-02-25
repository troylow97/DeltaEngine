/**********************************************************************************
* \file   PhysicsSystem.h
* \brief  The file contains logic for physics system
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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
    void PhysicsSystem::Dash(Player& p, RigidBody& r, Collider& c);
    void PhysicsSystem::Jump(Player& p, RigidBody& r, Collider& c);
    void PhysicsSystem::AttacksCooldown(Player& p);
    Vector2 m_gravity_amount{0, -60.0f};
    float JumpForce{4500.0f};
    float InitialJumpForce{4500.0f};
    float AttackCooldown{0.0f};
    bool StartAttackCooldown{false};
    int CurrentJumpTicks{0};
    int MaxJumpTicks{12};
    int CurrentDashTicks{0};
    int MaxDashTicks{10};
  END_DEFINE_SYSTEM(PhysicsSystem)
}
