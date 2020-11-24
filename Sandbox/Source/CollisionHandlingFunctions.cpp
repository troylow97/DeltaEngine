#include "CollisionHandlingFunctions.h"
#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"
#include "AI/AITools.h"

int counter = 0;
namespace DeltaEngine
{
  void ReduceHealth(EntityID& id1, int amount)
  {
      if (env.pECS->GetWorld().GetEntityManager().HasComponent<Health>(id1) && 
          !env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(id1).isInvulnerable)
      {
          env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(id1).CurrentHealth -= amount;
      }
  }

  void TakeDamage(EntityID& id1, EntityID& id2)
  {
    if (
      env.pECS->GetWorld().GetEntityManager().HasComponent<EntityType>(id1) &&
      env.pECS->GetWorld().GetEntityManager().HasComponent<EntityType>(id2)
    )
    {
      auto& type1 = env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(id1).type;
      auto& type2 = env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(id2).type;
      auto& hp1 = env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(id1);
      auto& hp2 = env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(id2);

      if (type1 != type2)
      {
        //Lancer Melee Attack
        if ((type1 == EntityCategory::E_LANCER_CHARGE || type2 == EntityCategory::E_LANCER_CHARGE) &&
          (type1 == EntityCategory::E_PLAYER || type2 == EntityCategory::E_PLAYER))
        {
          ReduceHealth(id1, 1);
          ReduceHealth(id2, 1);
          return;
        }

        //Player Detection Ranged Attack
        if ((type1 == EntityCategory::E_PLAYER_BULLET_DETECTION || type2 == EntityCategory::E_PLAYER_BULLET_DETECTION) &&
            (type1 == EntityCategory::E_ENEMY || type2 == EntityCategory::E_ENEMY))
        {
            EntityID target;
            if (type1 == EntityCategory::E_PLAYER_BULLET_DETECTION)
            {
                target = id2;
            }
            else
            {
                target = id1;
            }

           env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& bullet, EntityType et)
           {
               if (et.type == EntityCategory::E_PLAYER_BULLET)
               {
                   AITools::BulletTowardsEntity(bullet, target);
               }
           });

        }

        //Player Ranged Attack
        if ((type1 == EntityCategory::E_PLAYER_BULLET || type2 == EntityCategory::E_PLAYER_BULLET) &&
          (type1 == EntityCategory::E_ENEMY || type2 == EntityCategory::E_ENEMY))
        {
          ReduceHealth(id1, 5);
          ReduceHealth(id2, 5);
          return;
        }

        //Player Melee Attack
        if ((type1 == EntityCategory::E_PLAYER_PUNCH || type2 == EntityCategory::E_PLAYER_PUNCH) &&
          (type1 == EntityCategory::E_ENEMY || type2 == EntityCategory::E_ENEMY))
        {
          ReduceHealth(id1, 5);
          ReduceHealth(id2, 5);
          return;
        }

        //Player Dash
        if ((type1 == EntityCategory::E_PLAYER_DASH || type2 == EntityCategory::E_PLAYER_DASH) &&
            (type1 == EntityCategory::E_ENEMY || type2 == EntityCategory::E_ENEMY))
        {
            ReduceHealth(id1, 5);
            ReduceHealth(id2, 5);
            return;
        }

        //Enemy Collide with player
        if ((type1 == EntityCategory::E_ENEMY && type2 == EntityCategory::E_PLAYER) ||
          (type1 == EntityCategory::E_PLAYER && type2 == EntityCategory::E_ENEMY))
        {
          if (type1 == EntityCategory::E_PLAYER)
            ReduceHealth(id1, 5);
          else
            ReduceHealth(id2, 5);
        }
      }
    }
  }
}
