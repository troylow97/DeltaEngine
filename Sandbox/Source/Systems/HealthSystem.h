/**********************************************************************************
* \file   LifespanSystem.h
* \brief  The file contains the system for updating entities health
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "ECS/ECSModule.h"
#include <vector>
#include "Components/Health.h"

namespace DeltaEngine
{
  DEFINE_SYSTEM(HealthSystem, Health)
    void Update() override;
    void LateUpdate() override;
  private:
    void LimitCurrentHealthToMaxHealth(Health& hp);
    void PlayAttackedAnimation(EntityID id);
    void ResetAttackedAnimation(EntityID id);
  END_DEFINE_SYSTEM(HealthSystem)
}
