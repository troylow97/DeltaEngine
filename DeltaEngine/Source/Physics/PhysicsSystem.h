#pragma once
#include "Core/Math/Vector.h"
#include "CollisionSystem.h"
#include "Collider.h"

namespace DeltaEngine
{
    DEFINE_SYSTEM(PhysicsSystem, RigidBody, Transform, Collider)
    public:
        PhysicsSystem() = default;
        ~PhysicsSystem() = default;
        //CollisionSystem collision_system();

        virtual void PhysicsSystem::update() override;
        virtual void PhysicsSystem::late_update() override;
        void PhysicsSystem::Init();
        void PhysicsSystem::UpdateComponents();
        void PhysicsSystem::UpdateVelocity(Entity id)
        {
        	//ecs->get_component<Transform>(id).position = ecs->get_component<RigidBody>(id).Velocity;
        }
        void Gravity(Entity id)
        {
        	Vector2 Gravity = { 0,-2 };
        	//ecs->get_component<RigidBody>(id).Velocity - Gravity;
        }

    END_DEFINE_SYSTEM(PhysicsSystem)
}
