/**********************************************************************************
* \file   UpgradeSystem.h
* \brief  The file contains the system for getting upgrading the player's stat
* \author Chin, Clara,     100% Code Contribution
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
DEFINE_SYSTEM(UpgradeSystem, Player)
  void Initialize() override;
  void Update() override;
  void LateUpdate() override;
  void TempUpgradePointsUI();
  void UpgradeAttack();
  void UpgradeMaxHealth();
private:
  int PointsCollected;
  int Number_Of_Times_Attack_Upgraded;
  int Number_Of_Times_HP_Upgraded;
  EntityID UpgradePointsNumber;
  bool UpgradeTextFirstTime = true;
END_DEFINE_SYSTEM(UpgradeSystem)
}