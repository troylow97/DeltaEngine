#include "LifespanSystem.h"
#include "Core/GlobalStruct.h"
#include "Core/Debugging/Profiler/Profiler.h"
#include "Core/GameClock/GameClock.h"

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
              ls.Timer -= env.pClock->DeltaTime();
          }
      });
  }
	
  void LifespanSystem::Update()
  {
    DestroyedEntities.clear();
    UpdateLifespan();

    em.ForEach([&](EntityID& id, Health& hp, EntityType& et)
    {
      LimitCurrentHealthToMaxHealth(hp);
    	
      if (hp.CurrentHealth <= 0)
      {
        if (et.type == EntityCategory::E_PLAYER && env.pECS->GetWorld().GetEntityManager().HasComponent<Player>(id))
        {
          auto& p = env.pECS->GetWorld().GetEntityManager().GetComponent<Player>(id);
          p.isDead = true;
        }
        else
        {
          DestroyedEntities.push_back(id);
        }
      }
    });

    for (EntityID i : DestroyedEntities)
    {
      em.DestroyEntity(i);
    }
    Profiler::Instance().Record("Lifespan System");
  }

  void LifespanSystem::LateUpdate()
  {
  }
}
