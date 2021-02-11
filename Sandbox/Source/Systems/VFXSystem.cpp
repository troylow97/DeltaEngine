/**********************************************************************************
* \file   VFXSystem.cpp
* \brief  The file contains the system for getting upgrading the player's stat
* \author Chin, Clara,       0% Code Contribution
* \author Low , Troy ,       0% Code Contribution 
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "VFXSystem.h"
#include "Core/GlobalStruct.h"
#include "UnitManager.h"
#include "Core/Utils/Random.h"

namespace DeltaEngine
{
  void VFXSystem::Update()
  {
  	if(em.IsEntityValid(UnitManager::GetPlayerID()) && em.HasComponent<Attack>(UnitManager::GetPlayerID()) && em.HasComponent<Transform>(UnitManager::GetPlayerID()))
  	{
        auto& player_pos = em.GetComponent<Transform>(UnitManager::GetPlayerID());
        auto& player_attack = em.GetComponent<Attack>(UnitManager::GetPlayerID());

        if (player_attack.DamageEnemy)
        {
            ShowVFX++;

            if (ShowVFX == 3)
            {
                ShowHitVFX(player_pos.position, { 0.3f, 0.3f }, "Textures/DAVE_HITFX", "Animation/DaveHitVFX", 0.5f);
                player_attack.DamageEnemy = false;
                ShowVFX = 0;
            }
        }
  	}

  }
  
  void VFXSystem::LateUpdate()
  {
    // 
  }
  
  void VFXSystem::ShowHitVFX(Vector3 pos, Vector2 size, std::string image, std::string animation, /*std::string animation_bool,*/ float duration)
  {
    auto& player_id = env.pECS->GetWorld().GetEntityManager().GetComponent<EntityID>(UnitManager::GetPlayerID());

    EntityID vfx = em.CreateEntity<Animator, Renderer2D, Image, Lifespan, State>();
    float random_x = Random::RandomFloatRange(-0.2f, 0.2f);
    float random_y = Random::RandomFloatRange(-0.2f, 0.2f);

    if (em.GetComponent<Image>(player_id).m_FlipX == false)
    {
      em.GetComponent<Transform>(vfx).position.x = pos.x + 0.4f + random_x;
      em.GetComponent<Transform>(vfx).position.y = pos.y + random_y;
    }
    else
    {
      em.GetComponent<Transform>(vfx).position.x = pos.x - 0.4f + random_x;
      em.GetComponent<Transform>(vfx).position.y = pos.y + random_y;
    }
    em.GetComponent<Transform>(vfx).scale = size;
    em.GetComponent<Renderer2D>(vfx).m_SortingLayer = 6;
    em.GetComponent<Image>(vfx).m_Sprite.m_Key = image; // e.g. "Textures/DAVE_HITFX"
    em.GetComponent<Image>(vfx).m_Sprite.m_Index = 0;
    em.GetComponent<Image>(vfx).m_Size = size;
    em.GetComponent<Lifespan>(vfx).Timer = duration;
    em.GetComponent<EntityType>(vfx).type = EntityCategory::E_VFX;
    em.GetComponent<Animator>(vfx).m_ControllerKey = animation; // e.g. "Animation/DaveHitVFX"
    //em.GetComponent<State>(vfx).SetBool(animation_bool, true);
  }
}
