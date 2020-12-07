#include "UpgradeSystem.h"
#include "Core/GlobalStruct.h"
#include "UnitManager.h"

namespace DeltaEngine
{
  void UpgradeSystem::Initialize()
  {
  	//auto& p = env.pECS->GetWorld().GetEntityManager().GetComponent<Player>(UnitManager::GetPlayerID());
  	//PointsCollected = p.EnemiesDefeated * p.UpgradePoints;
  	//std::string str = std::to_string(PointsCollected);
    //
  	//EntityID UpgradePointsText = env.pECS->GetWorld().GetEntityManager().CreateEntity();
  	//env.pECS->GetWorld().GetEntityManager().AddComponent<Text>(UpgradePointsText);
  	//env.pECS->GetWorld().GetEntityManager().AddComponent<Renderer2D>(UpgradePointsText);
    //
  	//env.pECS->GetWorld().GetEntityManager().GetComponent<Text>(UpgradePointsText).m_Text = str;
  	//env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(UpgradePointsText).position = { Camera::editorCamera->Min(Camera::editorCameraTransform).x - 3.0f, Camera::editorCamera->Max(Camera::editorCameraTransform).y - 1.0f, 0.0f };
	//
	//UpgradePointsNumber = UpgradePointsText;
  }
    
  void UpgradeSystem::Update()
  {
	auto& p = env.pECS->GetWorld().GetEntityManager().GetComponent<Player>(UnitManager::GetPlayerID());
  	
	TempUpgradePointsUI();
	if (p.UpgradeAtk)
	{
	  PointsCollected = p.EnemiesDefeated * p.UpgradePoints;
	  if (PointsCollected >= p.PointsNeededForUpgrade)
	  {
		UpgradeAttack();
		p.UpgradedAtk = true;
	  }
	  p.UpgradeAtk = false;
	}
	if (p.UpgradeHP)
	{
	  PointsCollected = p.EnemiesDefeated * p.UpgradePoints;
	  if (PointsCollected >= p.PointsNeededForUpgrade)
	  {
		UpgradeMaxHealth();
		p.UpgradedHP = true;
	  }
	  p.UpgradeHP = false;
	}
  }
  
  void UpgradeSystem::LateUpdate()
  {
	// 
  }

  void UpgradeSystem::TempUpgradePointsUI()
  {
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