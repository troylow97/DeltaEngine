#pragma once
#include "ECS/ECSModule.h"
#include "Components/Transform.h"
#include "Components/Collider.h"
#include "Components/RigidBody.h"

namespace DeltaEngine
{
DEFINE_SYSTEM( PhysicsDrawSystem, Transform, Collider, RigidBody )
void PhysicsDrawSystem::Update() override;
void PhysicsDrawSystem::LateUpdate() override;
END_DEFINE_SYSTEM( PhysicsDrawSystem )
}
