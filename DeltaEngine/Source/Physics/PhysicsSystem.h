#pragma once
#include "Core/Math/Vector.h"
#include "CollisionSystem.h"
#include "Components/Collider.h"

namespace DeltaEngine
{
    DEFINE_SYSTEM(PhysicsSystem, RigidBody, Transform, Collider)
public:
    PhysicsSystem::PhysicsSystem() = default;
    ~PhysicsSystem() = default;
    //CollisionSystem collision_system();

    virtual void PhysicsSystem::Update() override;
    virtual void PhysicsSystem::LateUpdate() override;
    void PhysicsSystem::Init();
    void PhysicsSystem::UpdateComponents();

private:
    void PhysicsSystem::UpdateVelocity();
    void PhysicsSystem::Gravity();

    Vector2 m_gravity_amount;

    END_DEFINE_SYSTEM(PhysicsSystem)
}
