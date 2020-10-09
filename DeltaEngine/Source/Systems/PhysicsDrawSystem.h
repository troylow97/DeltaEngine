#pragma once
#include "ECS/ECSModule.h"
#include "Physics/Collider.h"
#include "Physics/RigidBody.h"
#include "Core/Math/Transform.h"

namespace DeltaEngine
{
    DEFINE_SYSTEM(PhysicsDrawSystem, Transform, Collider, RigidBody)
    void PhysicsDrawSystem::update() override;
    void PhysicsDrawSystem::late_update() override;
    END_DEFINE_SYSTEM(PhysicsDrawSystem)
}
