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