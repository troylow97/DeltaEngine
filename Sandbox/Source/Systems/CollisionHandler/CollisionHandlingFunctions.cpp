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
  
  bool CollisionHandlerFunctions::CheckEntityType(EntityID id1, EntityCategory typecheck1, EntityID id2,
      EntityCategory typecheck2)
  {
    auto& type1 = env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(id1).type;
    auto& type2 = env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(id2).type;
    return (type1 == typecheck1 && type2 == typecheck2);
  }
  
  EntityID GetEntityID(EntityID id1, EntityID id2, EntityCategory type)
  {
    auto& type1 = env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(id1).type;
    auto& type2 = env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(id2).type;
    (void)type2;

    if (type1 == type)
      return id1;
    
    return id2;
  }
  
  void CollisionHandlerFunctions::ApplyKnockBack(EntityID& defender, EntityID& attacker,const float amount)
  {
    const Vector2 kb_vector = env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(attacker).Velocity.Normalize();
    auto& rb = env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(defender);
    rb.AccumulatedForce += kb_vector * amount;
  }
  
  bool CollisionHandlerFunctions::CheckBlock(EntityID& defender, EntityID& attacker)
  {
    auto& block = env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(defender);
    
    const Vector2 kb_vector = env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(attacker).Velocity.Normalize();
    
    if (block.Blocking && ((kb_vector.x > 0 && AITools::isFacingLeft(defender)) || (kb_vector.x < 0 && AITools::isFacingRight(defender))))
    {
      return true;
    }
     return false;
  }
  
  void CollisionHandlerFunctions::TakeDamage(EntityID& id1, EntityID& id2)
  {
    auto& em = env.pECS->GetWorld().GetEntityManager();
    const auto& et1 = em.GetComponent<EntityType>(id1);
    const auto& et2 = em.GetComponent<EntityType>(id2);
    
    if (et1.type != et2.type)
    {
      //Fiddler Melee Attack
      {
        if (CheckEntityType(id1, EntityCategory::E_ENEMY_FIDDLER_PUNCH, id2, EntityCategory::E_PLAYER))
        {
          if(!CheckBlock(id2,id1))
            ReduceHealth(id2, static_cast<int>(CollisionHandlerFiddlerData.Damage));
          ApplyKnockBack(id2, id1,1000.0f);
          return;
        }
        if (CheckEntityType(id2, EntityCategory::E_ENEMY_FIDDLER_PUNCH, id1, EntityCategory::E_PLAYER))
        {
          if (!CheckBlock(id1, id2))
            ReduceHealth(id1, static_cast<int>(CollisionHandlerFiddlerData.Damage));
          ApplyKnockBack(id1, id2, 1000.0f);
          return;
        }
      }
    
      //Lancer Melee Attack
      {
        if (CheckEntityType(id1, EntityCategory::E_ENEMY_LANCER_PUNCH, id2, EntityCategory::E_PLAYER))
        {
          ReduceHealth(id2, static_cast<int>(CollisionHandlerLancerData.Damage));
          ApplyKnockBack(id2, id1, 600.0f);
          return;
        }
        if (CheckEntityType(id2, EntityCategory::E_ENEMY_LANCER_PUNCH, id1, EntityCategory::E_PLAYER))
        {
          ReduceHealth(id1, static_cast<int>(CollisionHandlerLancerData.Damage));
          ApplyKnockBack(id1, id2, 600.0f);
          return;
        }
      }
    
      //Serpentipede Ranged Attack
      {
        if (CheckEntityType(id1, EntityCategory::E_ENEMY_BULLET, id2, EntityCategory::E_PLAYER))
        {
          ReduceHealth(id2, static_cast<int>(CollisionHandlerSerpentipedeData.Damage));
          //ApplyKnockBack(id2, id1, 600.0f);
          return;
        }
        if (CheckEntityType(id2, EntityCategory::E_ENEMY_BULLET, id1, EntityCategory::E_PLAYER))
        {
          ReduceHealth(id1, static_cast<int>(CollisionHandlerSerpentipedeData.Damage));
          //ApplyKnockBack(id2, id1, 600.0f);
          return;
        }
      }
    
      //Player Detection Ranged Attack
      if (CheckEntityType(id1, EntityCategory::E_PLAYER_BULLET_DETECTION, id2, EntityCategory::E_ENEMY) ||
          CheckEntityType(id2, EntityCategory::E_PLAYER_BULLET_DETECTION, id1, EntityCategory::E_ENEMY))
      {
        EntityID target = GetEntityID(id1, id2, EntityCategory::E_ENEMY);
        
        env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& bullet, EntityType et)
        {
          if (et.type == EntityCategory::E_PLAYER_BULLET)
            AITools::BulletTowardsEntity(bullet, target);
        });
      }
    
      const auto player = UnitManager::GetPlayerID();
      auto& att = env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(player);
      //Player Ranged Attack
      if (CheckEntityType(id1, EntityCategory::E_PLAYER_BULLET, id2, EntityCategory::E_ENEMY) ||
          CheckEntityType(id2, EntityCategory::E_PLAYER_BULLET, id1, EntityCategory::E_ENEMY))
      {
        EntityID enemy = GetEntityID(id1, id2, EntityCategory::E_ENEMY);
        ReduceHealth(enemy, att.RangedDamage);
        if (env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(enemy).CurrentHealth <= 0)
          env.pECS->GetWorld().GetEntityManager().GetComponent<Player>(UnitManager::GetPlayerID()).EnemiesDefeated++;
        return;
      }
    
      //Player SMG Attack
      if (CheckEntityType(id1, EntityCategory::E_PLAYER_SMG, id2, EntityCategory::E_ENEMY) ||
        CheckEntityType(id2, EntityCategory::E_PLAYER_SMG, id1, EntityCategory::E_ENEMY))
      {
        EntityID enemy = GetEntityID(id1, id2, EntityCategory::E_ENEMY);
        ReduceHealth(enemy, att.SMGDamage);
        if (env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(enemy).CurrentHealth <= 0)
          env.pECS->GetWorld().GetEntityManager().GetComponent<Player>(UnitManager::GetPlayerID()).EnemiesDefeated++;
        return;
      }
  
      //Player Dash
      if (CheckEntityType(id1, EntityCategory::E_PLAYER_DASH, id2, EntityCategory::E_ENEMY) ||
        CheckEntityType(id2, EntityCategory::E_PLAYER_DASH, id1, EntityCategory::E_ENEMY))
      {
        EntityID enemy = GetEntityID(id1, id2, EntityCategory::E_ENEMY);
        ReduceHealth(enemy, 1); // 1
        if (env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(enemy).CurrentHealth <= 0)
          env.pECS->GetWorld().GetEntityManager().GetComponent<Player>(UnitManager::GetPlayerID()).EnemiesDefeated++;
      }
  
      //Player Melee Attack
      if (CheckEntityType(id1, EntityCategory::E_PLAYER_PUNCH, id2, EntityCategory::E_ENEMY) ||
          CheckEntityType(id2, EntityCategory::E_PLAYER_PUNCH, id1, EntityCategory::E_ENEMY))
      {
        const EntityID enemy = GetEntityID(id1, id2, EntityCategory::E_ENEMY);
        const EntityID punch = GetEntityID(id1, id2, EntityCategory::E_PLAYER_PUNCH);
        Vector2 kb_vector = em.GetComponent<RigidBody>(punch).Velocity.Normalize();
        att.DamageEnemy = true;
        if (att.NumberOfCombos == att.MaxComboNumber)
        {
          ReduceHealth(id1, att.MeleeComboDamage);
          ReduceHealth(id2, att.MeleeComboDamage);


          //ShowHitVFX(em.GetComponent<Transform>(id1).position,"Textures/DAVE_HITFX","Animation/DaveHitVFX");
          env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(enemy).AccumulatedForce += kb_vector * att.KnockbackComboAmount; // direction * force
          att.NumberOfCombos = 0;
        }
        env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(enemy).AccumulatedForce += kb_vector * att.KnockbackAmount;
        ReduceHealth(id1, att.MeleeDamage);
        ReduceHealth(id2, att.MeleeDamage);
        
        if (env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(enemy).CurrentHealth <= 0)
          env.pECS->GetWorld().GetEntityManager().GetComponent<Player>(UnitManager::GetPlayerID()).EnemiesDefeated++;
      }
    }
  }
}
