#pragma once
#include "ECS/ECSModule.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Collider.h"
#include "ECS/Components/RigidBody.h"

namespace DeltaEngine
{
    DEFINE_SYSTEM(PhysicsDrawSystem, Transform, Collider, RigidBody)
    void PhysicsDrawSystem::update() override;
    void PhysicsDrawSystem::late_update() override;
    END_DEFINE_SYSTEM(PhysicsDrawSystem)
}
