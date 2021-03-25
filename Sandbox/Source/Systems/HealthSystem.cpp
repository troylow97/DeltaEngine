/**********************************************************************************
* \file   LifespanSystem.cpp
* \brief  The file contains the system for updating entities health
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "HealthSystem.h"


#include "UnitManager.h"
#include "Audio/AudioEngine.h"
#include "Core/GlobalStruct.h"
#include "Core/Debugging/Profiler/Profiler.h"
#include "Core/GameClock/EngineClock.h"

namespace DeltaEngine
{
  static std::vector<EntityID> entities;

  void HealthSystem::LimitCurrentHealthToMaxHealth(Health& hp)
  {
    if (hp.CurrentHealth > hp.MaxHealth)
    {
      hp.CurrentHealth = hp.MaxHealth;
    }
  }

  void HealthSystem::Update()
  {
    Query q;
    q.Exclude<Lifespan>();
    em.ForEach(q, [&](EntityID& id, Health& hp, EntityType& et)
    {
      LimitCurrentHealthToMaxHealth(hp);
	  
      if (hp.CurrentHealth <= 0)
      {
        if (et.type == EntityCategory::E_ENEMY)
          entities.push_back(id);
        else if (et.type == EntityCategory::E_PLAYER)
          em.GetComponent<Player>(id).IsDead = true;
      } 
      else
      {
        auto& renderer = em.GetComponent<Renderer2D>(id);

        if (hp.isDamagedTimer > -0.1f)
        {
          renderer.m_Color = {1, 0, 0};
          hp.isDamagedTimer -= env.pClock->FixedDeltaTime();
          PlayAttackedAnimation(id);
        }
        else
        {
          renderer.m_Color = { 1, 1, 1 };
          em.GetComponent<State>(id).SetBool("IsAttacked", false);

        }

      }
    });

    for (auto& entity : entities)
    {
      em.GetComponent<State>(entity).SetBool("IsAlertRunning", false);
      em.GetComponent<State>(entity).SetBool("IsAttacked", true);
      em.GetComponent<State>(entity).SetBool("IsAttacked", false);
      em.GetComponent<State>(entity).SetBool("BeginCharging", false);
      em.GetComponent<State>(entity).SetBool("Charge", false);
      em.GetComponent<State>(entity).SetBool("MeleeAttack", false);    	
      em.GetComponent<State>(entity).SetBool("IsAlerted", false);
      em.GetComponent<State>(entity).SetBool("IsBouncing", false);
      em.GetComponent<State>(entity).SetBool("IsIdle", false);
      em.GetComponent<State>(entity).SetBool("IsDead", true);
      em.AddComponent<Lifespan>(entity);
      em.GetComponent<Lifespan>(entity).Timer = 1.0f;
      em.GetComponent<RigidBody>(entity).isMoveable = false;
    }
    entities.clear();

   



    Profiler::Instance().Record("Health System");
  }

  void HealthSystem::PlayAttackedAnimation(EntityID id)
  {
      auto& et_type = em.GetComponent<EntityType>(id);
      auto& state = em.GetComponent<State>(id);

	  if(et_type.type == EntityCategory::E_ENEMY || et_type.type == EntityCategory::E_PLAYER)
	  {
          state.SetBool("IsIdle", false);
          state.SetBool("IsAlerted", false);
          state.SetBool("IsAlertRunning", false);
          state.SetBool("MeleeAttack", false);
          state.SetBool("IsAttacked", true);
	  }
  }

  void HealthSystem::ResetAttackedAnimation(EntityID id)
  {
      auto& et_type = em.GetComponent<EntityType>(id);
      auto& state = em.GetComponent<State>(id);

      if (et_type.type == EntityCategory::E_ENEMY)
      {
          state.SetBool("IsAttacked", false);
      }
  }
	
  void HealthSystem::LateUpdate()
  {
  }
}
