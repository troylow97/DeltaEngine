#pragma once
#include "Core/Math/Vector.h"
#include "CollisionSystem.h"
#include "Collider.h"

namespace DeltaEngine
{
    DEFINE_SYSTEM(PhysicsSystem, RigidBody, Transform, Collider)
    public:
        PhysicsSystem::PhysicsSystem() = default;
        ~PhysicsSystem() = default;
        //CollisionSystem collision_system();

        virtual void PhysicsSystem::update() override;
        virtual void PhysicsSystem::late_update() override;
        void PhysicsSystem::UpdateComponents();

    private:
        void PhysicsSystem::UpdateVelocity();
        void PhysicsSystem::Gravity();

    END_DEFINE_SYSTEM(PhysicsSystem)
}
