#include "UpgradeSystem.h"
#include "Core/GlobalStruct.h"
#include "UnitManager.h"

namespace DeltaEngine
{
  void UpgradeSystem::Update()
  {
	auto& p = env.pECS->GetWorld().GetEntityManager().GetComponent<Player>(UnitManager::GetPlayerID());

	if (p.UpgradeAtk)
	{
	  PointsCollected = p.EnemiesDefeated * p.UpgradePoints;
	  if (PointsCollected >= p.PointsNeededForUpgrade)
	  {
		UpgradeAttack();
	  }
	  p.UpgradeAtk = false;
	}
	if (p.UpgradeHP)
	{
	  PointsCollected = p.EnemiesDefeated * p.UpgradePoints;
	  if (PointsCollected >= p.PointsNeededForUpgrade)
	  {
		UpgradeMaxHealth();
	  }
	  p.UpgradeHP = false;
	}
  }
  
  void UpgradeSystem::LateUpdate()
  {
	// 
  }

  void UpgradeSystem::UpgradeAttack()
  {
	Number_Of_Times_Attack_Upgraded++;

	em.ForEach([&](EntityID& id, Attack& a, Player& p)
	{
	  a.MeleeDamage += p.AttackUpgradeIncrease;
	  a.RangedDamage += p.AttackUpgradeIncrease;
	  p.EnemiesDefeated--;
	});
  }

  void UpgradeSystem::UpgradeMaxHealth()
  {
	Number_Of_Times_HP_Upgraded++;

	em.ForEach([&](EntityID& id, Health& h, Player& p)
	{
	  h.MaxHealth += p.HealthUpgradeIncrease;
	  p.EnemiesDefeated--;
	});
  }
}