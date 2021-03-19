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
#include "Core/GameClock/EngineClock.h"
#include "Core/GlobalStruct.h"

namespace DeltaEngine
{
  bool RespawnSystem::in_tutorial = true;
  bool RespawnSystem::in_level_1 = true;
  RespawnPoints RespawnSystem::respawns;

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
    Respawning();
    //DeatheEffect();
  }

  void RespawnSystem::LateUpdate()
  {
  }

  void RespawnSystem::CreateCheckpoints(int level)
  {
    JsonFile file;;
    if (level == 0) // tutorial
    {
      file.StartReader("Player/tutorial_respawn_points.json").LoadObject(respawns).EndReader();
      in_tutorial = true;
      in_level_1 = false;
    }
    if (level == 1) // tutorial
    {
      file.StartReader("Player/level1_respawn_points.json").LoadObject(respawns).EndReader();
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
    if (in_tutorial || in_level_1)
    {
      for (size_t i = 0; i < respawns.m_respawns.size(); i++)
      {
        //auto& em = GetEnv().pECS->GetWorld().GetEntityManager();
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
      in_tutorial = false;
      in_level_1 = false;
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
          if (t.position.x >= respawns.m_respawns[i].x && ((t.position.y - respawns.m_respawns[i].y) < 0.75f && (t.position.y - respawns.m_respawns[i].y) > -0.75f))
          {
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
              if (t.position.x >= t1.position.x && ((t.position.y - t1.position.y) < 0.75f && (t.position.y - t1.position.y) > -0.75f))
              {
                s1.SetBool("CheckpointReached", true);
              }
            }
          });
        }
      }
    }
  }

  void  RespawnSystem::Respawning()
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
        Transform& t = em.GetComponent<Transform>(id);
        Health& hp = em.GetComponent<Health>(id);
        State& s = em.GetComponent<State>(id);
        RigidBody& r = em.GetComponent<RigidBody>(id);

        if (p.IsDead)
        {
          s.SetBool("Dead", true);
          r.isMoveable = false;
          float temp_x = 1.0f, temp_y = 1.0f;
          for (size_t i = respawns.m_respawns.size(); i > 0; i--)
          {
            if (respawns.m_respawns[i - 1].z == 1.0f)
            {
              temp_x = respawns.m_respawns[i - 1].x;
              temp_y = respawns.m_respawns[i - 1].y;
              break;
            }
          }
          dying_countdown += env.pClock->FixedDeltaTime();
          if (dying_countdown > 3.0f)
          {
            t.position.x = temp_x;
            t.position.y = temp_y;
            hp.CurrentHealth = hp.MaxHealth;
            s.SetBool("Dead", false);
            s.SetBool("IsIdle", true);
            r.isMoveable = true;
            dying_countdown = 0.0f;
            p.IsDead = false;
          }
        }
      }
    }
  }

  void RespawnSystem::DeathEffect()
  {
    if (em.IsEntityValid(UnitManager::GetPlayerID()))
    {
      if (em.HasComponent<Player>(UnitManager::GetPlayerID()))
      {
        EntityID id = UnitManager::GetPlayerID();
       
        //Stop crash by checking for components	
        if (!em.HasComponent<Player>(id) || !em.HasComponent<Renderer2D>(id)
            || !em.HasComponent<State>(id))
          return;
       
        Player& p = em.GetComponent<Player>(id);
        Renderer2D& r = em.GetComponent<Renderer2D>(id);
        State& s = em.GetComponent<State>(id);

        if (p.IsDead)
        {
          //r.m_Color.a = 0.5f;
          s.SetBool("Dead", true);

          dying_countdown += env.pClock->FixedDeltaTime();
          if (dying_countdown > 10.0f)
          {
            // flickering effect
            
            //s.SetBool("Dead", false);
            //s.SetBool("IsIdle", true);
            //dying_countdown = 0.0f;
            //p.IsDead = false;
          }
          while (p.FadingCountdown > 0.0f)
          {
            p.FadingCountdown -= (env.pClock->FixedDeltaTime() * 0.1f);
            r.m_Color.a = /*(((*/p.FadingCountdown / p.FadingTimer/*) * 255.0f) / 255.0f) * 1.0f*/;
            std::cout << "r.m_Color.a is " << r.m_Color.a << std::endl;
          }
          p.IsDead = false;
          //Respawning();
        }
        p.FadingCountdown = p.FadingTimer;
      }
    }
  }
}
