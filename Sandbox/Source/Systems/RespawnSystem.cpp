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
  void RespawnSystem::Initialize()
  {
    //RespawnPoints res;
    //res.m_respawns.push_back({ 0,0 });

    JsonFile file;
    //rttr::variant v{ res };
    //auto& seq = v.create_sequential_view();
    //file.StartWriter("Player/respawn_points.json").StartObject().WriteObject(v).EndObject().EndWriter();

    //file.StartReader("Player/respawn_points.json").LoadObject(respawns).EndReader(); // respawn_points_clara
    file.StartReader("Player/respawn_points_clara.json").LoadObject(respawns).EndReader();
  }

  void RespawnSystem::Update()
  {
    DeathEffect();
    //Respawning();
  }

  void RespawnSystem::LateUpdate()
  {
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
          || !em.HasComponent<Health>(id) || !em.HasComponent<Image>(id))
          return;

        Player& p = em.GetComponent<Player>(id);
        Transform& t = em.GetComponent<Transform>(id);
        Health& hp = em.GetComponent<Health>(id);

        //if (p.IsDead)
        //{
          float temp_x = 0.541f, temp_y = 3.0f;
          float new_x = 0.0f, new_y = 0.0f;
          //if (AITools::isFacingRight(id))
          //{
          for (size_t i = 0; i < respawns.m_respawns.size(); i++)
          {
            if (t.position.x >= respawns.m_respawns[i].x)
            {
              temp_x = respawns.m_respawns[i].x;
              temp_y = respawns.m_respawns[i].y;
            }
          }
          new_x = temp_x;
          new_y = temp_y;
          t.position.x = new_x;
          t.position.y = new_y;
          hp.CurrentHealth = hp.MaxHealth;
          p.IsDead = false;
          //}
          //else if (AITools::isFacingLeft(id))
          //{
          //  for (int i = static_cast<int>(respawns.m_respawns.size() - 1); i >= 0; i--)
          //  {
          //    if (t.position.x <= respawns.m_respawns[i].x)
          //    {
          //      temp_x = respawns.m_respawns[i].x;
          //      temp_y = respawns.m_respawns[i].y;
          //    }
          //  }
          //  new_x = temp_x;
          //  new_y = temp_y;
          //  t.position.x = new_x;
          //  t.position.y = new_y;
          //  hp.CurrentHealth = hp.MaxHealth;
          //  p.IsDead = false;
          //}
        //}
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
        if (!em.HasComponent<Player>(id) || !em.HasComponent<Renderer2D>(id))
          return;
       
        Player& p = em.GetComponent<Player>(id);
        Renderer2D& r = em.GetComponent<Renderer2D>(id);

        if (p.IsDead)
        {
          //r.m_Color.a = 0.5f;
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
