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
	  UpgradeAttack();
	  p.UpgradeAtk = false;
	}
	if (p.UpgradeHP)
	{
	  UpgradeMaxHealth();
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
	});
  }

  void UpgradeSystem::UpgradeMaxHealth()
  {
	Number_Of_Times_HP_Upgraded++;

	em.ForEach([&](EntityID& id, Health& h, Player& p)
	{
	  h.MaxHealth += p.HealthUpgradeIncrease;
	});
  }
}