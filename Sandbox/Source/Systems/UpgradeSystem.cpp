#include "UpgradeSystem.h"
#include "Core/GlobalStruct.h"
#include "UnitManager.h"

namespace DeltaEngine
{
  void UpgradeSystem::Update()
  {
	// 
  }
  
  void UpgradeSystem::LateUpdate()
  {
	// 
  }

  void UpgradeSystem::TotalUpgradePoints()
  {
	auto& p = env.pECS->GetWorld().GetEntityManager().GetComponent<Player>(UnitManager::GetPlayerID());

	PointsCollected = p.EnemiesDefeated * p.UpgradePoints;
  }

  void UpgradeSystem::UpgradeAttack()
  {
	//
  }

  void UpgradeSystem::UpgradeMaxHealth()
  {
	//
  }
}