/**********************************************************************************
* \file   PhysicsDrawSystem.h
* \brief  The file contains BLAHBLAHBLAH
* \author Chin, Clara,   X% Code Contribution
* \author Low, Troy,     X% Code Contribution
* \author Ong, Graeme,   X% Code Contribution
* \author Tan, Tong Wee, X% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "ECS/ECSModule.h"
#include "Components/Transform.h"
#include "Components/Collider.h"
#include "Components/RigidBody.h"

namespace DeltaEngine
{
  DEFINE_SYSTEM(PhysicsDrawSystem, Transform, Collider, RigidBody)
    void PhysicsDrawSystem::Update() override;
    void PhysicsDrawSystem::LateUpdate() override;
    static inline bool gizmo;
  END_DEFINE_SYSTEM(PhysicsDrawSystem)
}
