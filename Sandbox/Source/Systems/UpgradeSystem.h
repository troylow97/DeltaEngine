#pragma once
#include "ECS/ECSModule.h"

namespace DeltaEngine
{
DEFINE_SYSTEM(UpgradeSystem, Player)
  void Update() override;
  void LateUpdate() override;
  void TotalUpgradePoints();
  void UpgradeAttack();
  void UpgradeMaxHealth();
private:
  int PointsCollected;
END_DEFINE_SYSTEM(UpgradeSystem)
}