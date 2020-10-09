#pragma once
#include "ECS/ECSModule.h"
#include "ECS/Components/Collider.h"
#include "ECS/Components/Transform.h"
namespace DeltaEngine
{
    DEFINE_SYSTEM(PhysicsDrawSystem, Transform, Collider)
    void PhysicsDrawSystem::update() override;
    void PhysicsDrawSystem::late_update() override;
    END_DEFINE_SYSTEM(PhysicsDrawSystem)
}
