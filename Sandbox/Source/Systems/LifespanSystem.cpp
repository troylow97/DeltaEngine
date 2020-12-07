#include "LifespanSystem.h"

#include "UnitManager.h"
#include "Audio/AudioEngine.h"
#include "Core/GlobalStruct.h"
#include "Core/Debugging/Profiler/Profiler.h"
#include "Core/GameClock/EngineClock.h"

namespace DeltaEngine
{
  void LifespanSystem::LimitCurrentHealthToMaxHealth(Health& hp)
  {
      if (hp.CurrentHealth > hp.MaxHealth)
      {
          hp.CurrentHealth = hp.MaxHealth;
      }
  }

  void LifespanSystem::UpdateLifespan()
  {
      em.ForEach([&](EntityID& id, Lifespan& ls)
      {
          if (ls.Timer < 0)
          {
              DestroyedEntities.push_back(id);
          }
          else
          {
              ls.Timer -= env.pClock->FixedDeltaTime();
          }
      });
  }
	
  void LifespanSystem::Update()
  {
    em.ForEach([&](EntityID& id, Health& hp, EntityType& et)
    {
        LimitCurrentHealthToMaxHealth(hp);
        auto& player = env.pECS->GetWorld().GetEntityManager().GetComponent<Player>(UnitManager::GetPlayerID());

        if (hp.CurrentHealth <= 0)
        {
          if (env.pECS->GetWorld().GetEntityManager().HasComponent<Player>(id))
          {
              auto& p = env.pECS->GetWorld().GetEntityManager().GetComponent<Player>(id);
              p.IsDead = true;
          }
          else
          {
			  DestroyedEntities.push_back(id);
          }
        }
        else
        {
        	if(em.HasComponent<Renderer2D>(id))
        	{
                if (hp.isDamagedTimer > 0.0f)
                {
                    em.GetComponent<Renderer2D>(id).m_Color = { 1,0,0 };
                    hp.isDamagedTimer -= env.pClock->FixedDeltaTime();
                }
                else
                {
                    em.GetComponent<Renderer2D>(id).m_Color = { 1,1,1 };
                }
        	}
        }
    });

    for (EntityID i : DestroyedEntities) //having a duplicate of this is necessary for now
    {
        em.DestroyEntity(i);
    }
    DestroyedEntities.clear();
  	
    UpdateLifespan();

    for (EntityID i : DestroyedEntities)
    {
    	
    	em.DestroyEntity(i);
    }

    DestroyedEntities.clear();
  	
    Profiler::Instance().Record("Lifespan System");
  }

  void LifespanSystem::LateUpdate()
  {
  }
}
