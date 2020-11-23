#include "CollisionHandlingFunctions.h"
#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"

namespace DeltaEngine
{
  void TakeDamage(EntityID& id1, EntityID& id2)
  {
    if (env.pECS->GetWorld().GetEntityManager().HasComponent<Health>(id1) &&
      env.pECS->GetWorld().GetEntityManager().HasComponent<EntityType>(id1) &&
      env.pECS->GetWorld().GetEntityManager().HasComponent<Health>(id2) &&
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
          hp1.CurrentHealth -= 5;
          hp2.CurrentHealth -= 5;
          return;
        }

        //Player Ranged Attack
        if ((type1 == EntityCategory::E_PLAYER_BULLET || type2 == EntityCategory::E_PLAYER_BULLET) &&
          (type1 == EntityCategory::E_ENEMY || type2 == EntityCategory::E_ENEMY))
        {
          hp1.CurrentHealth--;
          hp2.CurrentHealth--;
          return;
        }

        //Player Melee Attack
        if ((type1 == EntityCategory::E_PLAYER_PUNCH || type2 == EntityCategory::E_PLAYER_PUNCH) &&
          (type1 == EntityCategory::E_ENEMY || type2 == EntityCategory::E_ENEMY))
        {
          hp1.CurrentHealth--;
          hp2.CurrentHealth--;
          return;
        }

        //Player Dash
        if ((type1 == EntityCategory::E_PLAYER_DASH || type2 == EntityCategory::E_PLAYER_DASH) &&
            (type1 == EntityCategory::E_ENEMY || type2 == EntityCategory::E_ENEMY))
        {
            hp1.CurrentHealth--;
            hp2.CurrentHealth--;
            return;
        }

        //Enemy Collide with player
        if ((type1 == EntityCategory::E_ENEMY && type2 == EntityCategory::E_PLAYER) ||
          (type1 == EntityCategory::E_PLAYER && type2 == EntityCategory::E_ENEMY))
        {
          if (type1 == EntityCategory::E_PLAYER)
            hp1.CurrentHealth--;
          else
            hp2.CurrentHealth--;
        }
      }
    }
  }
}
