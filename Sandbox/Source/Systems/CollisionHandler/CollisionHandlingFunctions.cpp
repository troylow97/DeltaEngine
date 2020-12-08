/**********************************************************************************
* \file   CollisionHandlingFunctions.cpp
* \brief  The file contains functions that would be run during Collision Handling
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "CollisionHandlingFunctions.h"
#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"
#include "../AI/AITools.h"
#include "../UnitManager.h"

namespace DeltaEngine
{
    EnemyData CollisionHandlerFunctions::CollisionHandlerFiddlerData{};
    EnemyData CollisionHandlerFunctions::CollisionHandlerLancerData{};
    EnemyData CollisionHandlerFunctions::CollisionHandlerSerpentipedeData{};
	
    void CollisionHandlerFunctions::Initialise()
    {
        JsonFile file;
        file.StartReader("Enemy/Lancer.json").LoadObject(CollisionHandlerLancerData).EndReader();
        JsonFile file2;
        file.StartReader("Enemy/Fiddler.json").LoadObject(CollisionHandlerFiddlerData).EndReader();
        JsonFile file3;
        file.StartReader("Enemy/Serpentipede.json").LoadObject(CollisionHandlerSerpentipedeData).EndReader();
    }
    
    void CollisionHandlerFunctions::ReduceHealth(EntityID& id1, int amount)
    {
        if (env.pECS->GetWorld().GetEntityManager().HasComponent<Health>(id1) && 
            !env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(id1).isInvulnerable)
        {
            env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(id1).CurrentHealth -= amount;
            env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(id1).isDamagedTimer = 0.3f;
        }
    }
    
    bool CollisionHandlerFunctions::CheckEntityType(EntityID id1,EntityCategory typecheck1,EntityID id2, EntityCategory typecheck2)
    {
        auto& type1 = env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(id1).type;
        auto& type2 = env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(id2).type;
        return ((type1 == typecheck1 || type2 == typecheck1) && (type1 == typecheck2 || type2 == typecheck2));
    }
    
    EntityID GetEntityID(EntityID id1, EntityID id2,EntityCategory type)
    {
        auto& type1 = env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(id1).type;
        auto& type2 = env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(id2).type;
        if (type1 == type)
            return id1;
    
        return id2;
    }

    void CollisionHandlerFunctions::TakeDamage(EntityID& id1, EntityID& id2)
    {
      if (
          env.pECS->GetWorld().GetEntityManager().HasComponent<Health>(id1) &&
          env.pECS->GetWorld().GetEntityManager().HasComponent<Health>(id2) &&
        env.pECS->GetWorld().GetEntityManager().HasComponent<EntityType>(id1) &&
        env.pECS->GetWorld().GetEntityManager().HasComponent<EntityType>(id2) 
      )
      {
        const EntityID player = UnitManager::GetPlayerID();
        auto& type1 = env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(id1).type;
        auto& type2 = env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(id2).type;
    
        if (type1 != type2)
        {
            //Fiddler Melee Attack
            if (CheckEntityType(id1,EntityCategory::E_ENEMY_FIDDLER_PUNCH,id2,EntityCategory::E_PLAYER))
            {
                ReduceHealth(id1, (int)CollisionHandlerFiddlerData.Damage);
                ReduceHealth(id2, (int)CollisionHandlerFiddlerData.Damage);
                return;
            }

            //Lancer Melee Attack
            if (CheckEntityType(id1, EntityCategory::E_ENEMY_LANCER_PUNCH, id2, EntityCategory::E_PLAYER))
            {
                ReduceHealth(id1, (int)CollisionHandlerLancerData.Damage);
                ReduceHealth(id2, (int)CollisionHandlerLancerData.Damage);
                return;
            }

        	//Serpentipede Ranged Attack
            if (CheckEntityType(id1, EntityCategory::E_ENEMY_BULLET, id2, EntityCategory::E_PLAYER))
            {
                ReduceHealth(id1, (int)CollisionHandlerSerpentipedeData.Damage);
                ReduceHealth(id2, (int)CollisionHandlerSerpentipedeData.Damage);
                return;
            }
    
            //Player Detection Ranged Attack
            if (CheckEntityType(id1, EntityCategory::E_PLAYER_BULLET_DETECTION, id2, EntityCategory::E_ENEMY))
            {
                EntityID target = GetEntityID(id1, id2, EntityCategory::E_ENEMY);
    
               env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& bullet, EntityType et)
               {
                   if (et.type == EntityCategory::E_PLAYER_BULLET)
                   {
                       AITools::BulletTowardsEntity(bullet, target);
                   }
               });
            }
        	
        	if(env.pECS->GetWorld().GetEntityManager().HasComponent<Attack>(player))
        	{
              auto& attack = env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(player);
              //Player Ranged Attack
              if (CheckEntityType(id1, EntityCategory::E_PLAYER_BULLET, id2, EntityCategory::E_ENEMY))
              {
                  const EntityID enemy = GetEntityID(id1, id2, EntityCategory::E_ENEMY);
                  ReduceHealth(id1, attack.RangedDamage);
                  ReduceHealth(id2, attack.RangedDamage);
                  if (env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(enemy).CurrentHealth <= 0)
                      env.pECS->GetWorld().GetEntityManager().GetComponent<Player>(UnitManager::GetPlayerID()).EnemiesDefeated++;
                  return;
              }
    
              //Player Melee Attack
              if (CheckEntityType(id1, EntityCategory::E_PLAYER_PUNCH, id2, EntityCategory::E_ENEMY))
              {
                  const EntityID enemy = GetEntityID(id1, id2, EntityCategory::E_ENEMY);
                  const EntityID punch = GetEntityID(id1, id2, EntityCategory::E_PLAYER_PUNCH);
                  auto& a = env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(UnitManager::GetPlayerID());
                  Vector2 kb_vector = env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(punch).Velocity.Normalize();
                  if (attack.NumberOfCombos == attack.MaxComboNumber)
                  {
                      ReduceHealth(id1, attack.MeleeComboDamage);
                      ReduceHealth(id2, attack.MeleeComboDamage);
                      env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(enemy).AccumulatedForce += kb_vector * a.KnockbackComboAmount; // direction * force
                      attack.NumberOfCombos = 0;
                  }
                  env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(enemy).AccumulatedForce += kb_vector * a.KnockbackAmount;
                  ReduceHealth(id1, attack.MeleeDamage);
                  ReduceHealth(id2, attack.MeleeDamage);

              	  if(env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(enemy).CurrentHealth <= 0)
                      env.pECS->GetWorld().GetEntityManager().GetComponent<Player>(UnitManager::GetPlayerID()).EnemiesDefeated++;
                  return;
              }
        	}
    
    	 //Player Dash
          if (CheckEntityType(id1, EntityCategory::E_PLAYER_DASH, id2, EntityCategory::E_ENEMY))
          {
              const EntityID enemy = GetEntityID(id1, id2, EntityCategory::E_ENEMY);
              ReduceHealth(id1, 1);
              ReduceHealth(id2, 1);
              if (env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(enemy).CurrentHealth <= 0)
                  env.pECS->GetWorld().GetEntityManager().GetComponent<Player>(UnitManager::GetPlayerID()).EnemiesDefeated++;
          }
    
          //Enemy Collide with player
          //if (CheckEntityType(id1, EntityCategory::E_PLAYER, id2, EntityCategory::E_ENEMY))
          //{
          //  if (type1 == EntityCategory::E_PLAYER)
          //    ReduceHealth(id1, 1);
          //}
        }
      }
    }
}
