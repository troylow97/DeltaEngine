#pragma once
#include "ECS/ECSModule.h"
#include "Physics/Collider.h"
#include "Core/Math/Transform.h"

namespace DeltaEngine
{
    DEFINE_SYSTEM(PhysicsDrawSystem, Transform, Collider)
    void PhysicsDrawSystem::update() override;
    void PhysicsDrawSystem::late_update() override;
    END_DEFINE_SYSTEM(PhysicsDrawSystem)
}
