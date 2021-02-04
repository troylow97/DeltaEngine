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
#include "Components/Lifespan.h"
#include <vector>

namespace DeltaEngine
{
  DEFINE_SYSTEM(LifespanSystem, Lifespan)
    void Update() override;
    void LateUpdate() override;
  private:
    std::vector<EntityID> DestroyedEntities;
  END_DEFINE_SYSTEM(LifespanSystem)
}
