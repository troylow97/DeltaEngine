/**********************************************************************************
* \file   VFXSystem.h
* \brief  The file contains the system for getting upgrading the player's stat
* \author Chin, Clara,       0% Code Contribution
* \author Low , Troy ,       0% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "ECS/ECSModule.h"

namespace DeltaEngine
{
  DEFINE_SYSTEM(VFXSystem, Player)
    void Update() override;
    void LateUpdate() override;

    void ShowHitVFX(Vector2 pos, std::string image, std::string animation);
private:
    //
  END_DEFINE_SYSTEM(VFXSystem)
}
