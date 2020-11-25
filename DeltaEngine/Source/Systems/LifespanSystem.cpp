#include "LifespanSystem.h"
#include "Core/GlobalStruct.h"
#include "Core/Debugging/Profiler/Profiler.h"
#include "Core/GameClock/GameClock.h"

namespace DeltaEngine
{
  void LifespanSystem::Update()
  {
    DestroyedEntities.clear();

    em.ForEach([&](EntityID& id, Lifespan& ls)
    {
      if (ls.Timer < 0)
      {
        em.DestroyEntity(id);
      }
      else
      {
        ls.Timer -= env.pClock->DeltaTime();
      }
    });

    em.ForEach([&](EntityID& id, Health& hp, EntityType& et, Player& p)
    {
      if (hp.CurrentHealth > hp.MaxHealth)
      {
        hp.CurrentHealth = hp.MaxHealth;
      }

      if (hp.CurrentHealth <= 0)
      {
        p.IsDead = true;
        DestroyedEntities.push_back(id);
      }
    });
    em.ForEach([&](EntityID& id, Health& hp, EntityType& et)
    {
      if (hp.CurrentHealth > hp.MaxHealth)
      {
        hp.CurrentHealth = hp.MaxHealth;
      }
      
      if (hp.CurrentHealth <= 0)
      {
        if (et.type == EntityCategory::E_ENEMY)
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
