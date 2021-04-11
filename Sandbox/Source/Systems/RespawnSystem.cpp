/**********************************************************************************
* \file   RespawnSystem.cpp
* \brief  The file contains the system for respawning the player
* \author Chin, Clara,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "../../Sandbox/Source/Systems/RespawnSystem.h"
#include "AI/AITools.h"
#include "UnitManager.h"
#include "LevelManager/LevelManager.h"
#include "../GameState.h"

namespace DeltaEngine
{
  bool RespawnSystem::opening_tutorial = true;
  bool RespawnSystem::opening_level_1 = true;
  bool RespawnSystem::in_tutorial = false;
  bool RespawnSystem::in_level_1 = false;
  int RespawnSystem::checkpoint_passed = 0;
  RespawnPoints RespawnSystem::respawns;
  Vector2 RespawnSystem::player_initial_position = { 0.0f, 0.0f };
  Vector2 RespawnSystem::player_spawning_position = { 0.0f, 0.0f };

  void RespawnSystem::Initialize()
  {
    ////RespawnPoints res;
    ////res.m_respawns.push_back({ 0,0,0 });
    //
    //JsonFile file;
    ////rttr::variant v{ res };
    ////auto& seq = v.create_sequential_view();
    ////file.StartWriter("Player/respawn_points.json").StartObject().WriteObject(v).EndObject().EndWriter();
    //
    ////file.StartReader("Player/respawn_points.json").LoadObject(respawns).EndReader(); // respawn_points_clara
    //file.StartReader("Player/respawn_points_clara.json").LoadObject(respawns).EndReader();
  }

  void RespawnSystem::Update()
  {
    CheckpointsLightUp();
    RefillHealth();
    DeathAnimation();
    ClearScreen();
    PassedCheckpointsLightUp();
    Respawning();
  }

  void RespawnSystem::LateUpdate()
  {
  }

  //void RespawnSystem::temp_create_checkpoints()
  //{
  //    EntityID id = UnitManager::GetPlayerID();
  //
  //    //Stop crash by checking for components	
  //    if (!em.HasComponent<Player>(id) || !em.HasComponent<Transform>(id))
  //        return;
  //
  //    Transform& t = em.GetComponent<Transform>(id);
  //    player_initial_position = { t.position.x, t.position.y };
  //
  //    for (size_t i = 0; i < respawns.m_respawns.size(); i++)
  //    {
  //        //auto& em = GetEnv().pECS->GetWorld().GetEntityManager();
  //        EntityID checkpoints = em.CreateEntity<Animator, Renderer2D, Image, State>();
  //        em.GetComponent<EntityName>(checkpoints).name = "Checkpoint";
  //
  //        em.GetComponent<Transform>(checkpoints).position.x = respawns.m_respawns[i].x;
  //        em.GetComponent<Transform>(checkpoints).position.y = respawns.m_respawns[i].y;
  //        em.GetComponent<Transform>(checkpoints).scale = { 1.0f, 1.0f, 1.0f };
  //
  //        em.GetComponent<Renderer2D>(checkpoints).m_SortingLayer = 3;
  //        em.GetComponent<Image>(checkpoints).m_Sprite.m_Key = "Textures/CHECKPOINT_OFF"; // e.g. "Textures/DAVE_HITFX"
  //        em.GetComponent<Image>(checkpoints).m_Sprite.m_Index = 0;
  //        em.GetComponent<Image>(checkpoints).m_Size = { 1.0f, 1.0f };
  //        em.GetComponent<EntityType>(checkpoints).type = EntityCategory::E_CHECKPOINT;
  //        em.GetComponent<Animator>(checkpoints).m_ControllerKey = "Animation/Checkpoint"; // e.g. "Animation/DaveHitVFX"
  //        em.GetComponent<State>(checkpoints).SetBool("CheckpointReached", false);
  //    }
  //}

  void RespawnSystem::CreateCheckpoints(int level)
  {
    JsonFile file;;
    if (level == 0) // tutorial
    {
      //file.StartReader("Player/tutorial_respawn_points.json").LoadObject(respawns).EndReader();
      //opening_tutorial = true;
      //opening_level_1 = false;
      //in_tutorial = true;
      //in_level_1 = false;
    }
    if (level == 1) // tutorial
    {
      file.StartReader("Player/level1_respawn_points.json").LoadObject(respawns).EndReader();
      opening_tutorial = false;
      opening_level_1 = true;
      in_tutorial = false;
      in_level_1 = true;
    }
    auto& em = GetEnv().pECS->GetWorld().GetEntityManager();
    em.ForEach([&](EntityID id1, EntityType& et1)
    {
      if (et1.type == EntityCategory::E_CHECKPOINT)
      {
        em.DestroyEntity(id1);
      }
    });
    if (opening_tutorial || opening_level_1)
    {
      EntityID id = UnitManager::GetPlayerID();
      
      //Stop crash by checking for components	
      if (!em.HasComponent<Player>(id) || !em.HasComponent<Transform>(id))
        return;
      
      Transform& t = em.GetComponent<Transform>(id);
      player_initial_position = { t.position.x, t.position.y };

      if (!checkpoint_passed)
        player_spawning_position = player_initial_position;

      for (size_t i = 0; i < respawns.m_respawns.size(); i++)
      {
        EntityID checkpoints = em.CreateEntity<Animator, Renderer2D, Image, State>();
        em.GetComponent<EntityName>(checkpoints).name = "Checkpoint";
        
        em.GetComponent<Transform>(checkpoints).position.x = respawns.m_respawns[i].x;
        em.GetComponent<Transform>(checkpoints).position.y = respawns.m_respawns[i].y;
        em.GetComponent<Transform>(checkpoints).scale = { 1.0f, 1.0f, 1.0f };
        
        em.GetComponent<Renderer2D>(checkpoints).m_SortingLayer = 3;
        em.GetComponent<Image>(checkpoints).m_Sprite.m_Key = "Textures/CHECKPOINT_OFF"; // e.g. "Textures/DAVE_HITFX"
        em.GetComponent<Image>(checkpoints).m_Sprite.m_Index = 0;
        em.GetComponent<Image>(checkpoints).m_Size = { 1.0f, 1.0f };
        em.GetComponent<EntityType>(checkpoints).type = EntityCategory::E_CHECKPOINT;
        em.GetComponent<Animator>(checkpoints).m_ControllerKey = "Animation/Checkpoint"; // e.g. "Animation/DaveHitVFX"
        em.GetComponent<State>(checkpoints).SetBool("CheckpointReached", false);
      }
      opening_tutorial = false;
      opening_level_1 = false;
    }
  }

  void RespawnSystem::CheckpointsLightUp()
  {
    if (em.IsEntityValid(UnitManager::GetPlayerID()))
    {
      if (em.HasComponent<Player>(UnitManager::GetPlayerID()))
      {
        EntityID id = UnitManager::GetPlayerID();
        
        //Stop crash by checking for components	
        if (!em.HasComponent<Player>(id) || !em.HasComponent<Transform>(id)
            || !em.HasComponent<Health>(id) || !em.HasComponent<Image>(id))
            return;

        Transform& t = em.GetComponent<Transform>(id);
        
        for (size_t i = 0; i < respawns.m_respawns.size(); i++)
        {
          if (((t.position.x - respawns.m_respawns[i].x) < 0.1f && (t.position.x - respawns.m_respawns[i].x) > -0.1f)
              && ((t.position.y - respawns.m_respawns[i].y) < 1.0f && (t.position.y - respawns.m_respawns[i].y) > -1.0f)
              && respawns.m_respawns[i].z != 1.0f)
          {
            ++checkpoint_passed;
            need_refill_health = true;
            player_spawning_position.x = respawns.m_respawns[i].x;
            player_spawning_position.y = respawns.m_respawns[i].y;
            respawns.m_respawns[i].z = 1.0f;
          }
        }

        for (size_t i = 0; i < respawns.m_respawns.size(); i++)
        {
          Vector2 checkpoint_coordinates = { respawns.m_respawns[i].x, respawns.m_respawns[i].y };
          env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, EntityType& et1, State& s1, Transform& t1)
          {
            if (et1.type == EntityCategory::E_CHECKPOINT)
            {
              if (t.position.x >= t1.position.x && ((t.position.y - t1.position.y) < 1.0f && (t.position.y - t1.position.y) > -1.0f))
              {
                s1.SetBool("CheckpointReached", true);
              }
            }
          });
        }
      }
    }
  }

  void RespawnSystem::PassedCheckpointsLightUp()
  {
    if (respawn_now)
    {
      for (size_t i = 0; i < checkpoint_passed; i++)
      {
        respawns.m_respawns[i].z = 1.0f;
      }
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, EntityType& et1, State& s1, Transform& t1)
      {
        if (et1.type == EntityCategory::E_CHECKPOINT && t1.position.z == 1.0f)
        {
          s1.SetBool("CheckpointReached", true);
        }
      });
    }
  }

  void RespawnSystem::RefillHealth()
  {
    if (need_refill_health)
    {
      EntityID id = UnitManager::GetPlayerID();
      Health& hp = em.GetComponent<Health>(id);
      hp.CurrentHealth = hp.MaxHealth;
      need_refill_health = false;
    }
  }

  void RespawnSystem::DeathAnimation()
  {
    if (em.IsEntityValid(UnitManager::GetPlayerID()))
    {
      if (em.HasComponent<Player>(UnitManager::GetPlayerID()))
      {
        EntityID id = UnitManager::GetPlayerID();
        
        //Stop crash by checking for components	
        if (!em.HasComponent<Player>(id) || !em.HasComponent<Transform>(id)
            || !em.HasComponent<Health>(id) || !em.HasComponent<Image>(id)
            || !em.HasComponent<State>(id) || !em.HasComponent<RigidBody>(id))
            return;
        
        Player& p = em.GetComponent<Player>(id);
        State& s = em.GetComponent<State>(id);
        RigidBody& r = em.GetComponent<RigidBody>(id);
        
        if (p.IsDead)
        {
          s.SetBool("Dead", true);
          r.isMoveable = false;
          
          dying_countdown += env.pClock->FixedDeltaTime();
        }
      }
    }
  }

  void RespawnSystem::ClearScreen()
  {
    if (dying_countdown > 3.0f)
    {
      if (in_tutorial)
      {
        GameStateLoad(GameState::TUTORIAL);
        respawn_now = true;
      }
      else if (in_level_1)
      {
        GameStateLoad(GameState::LEVEL_1);
        respawn_now = true;
      }
    }
  }

  void RespawnSystem::Respawning()
  {
    if (em.IsEntityValid(UnitManager::GetPlayerID()))
    {
      if (em.HasComponent<Player>(UnitManager::GetPlayerID()))
      {
         EntityID id = UnitManager::GetPlayerID();
        
         //Stop crash by checking for components	
         if (!em.HasComponent<Player>(id) || !em.HasComponent<Transform>(id)
             || !em.HasComponent<Health>(id) || !em.HasComponent<State>(id)
             || !em.HasComponent<RigidBody>(id))
           return;
         if (respawn_now)
         {
           Player& p = em.GetComponent<Player>(id);
           Transform& t = em.GetComponent<Transform>(id);
           Health& hp = em.GetComponent<Health>(id);
           State& s = em.GetComponent<State>(id);
           RigidBody& r = em.GetComponent<RigidBody>(id);

           t.position.x = player_spawning_position.x;
           t.position.y = player_spawning_position.y;
           hp.CurrentHealth = hp.MaxHealth;
           s.SetBool("Dead", false);
           s.SetBool("IsIdle", true);
           r.isMoveable = true;
           dying_countdown = 0.0f;
           p.IsDead = false;

           respawn_now = false;
         }
      }
    }
  }
}
