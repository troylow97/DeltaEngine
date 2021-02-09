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

namespace DeltaEngine
{
  void VFXSystem::Update()
  {
    //

  }
  
  void VFXSystem::LateUpdate()
  {
    // 
  }
  
  void VFXSystem::ShowHitVFX(Vector2 pos, std::string image, std::string animation)
  {
    auto& em = env.pECS->GetWorld().GetEntityManager();
    EntityID vfx = em.CreateEntity<Animator, Renderer2D, Image>();
    em.GetComponent<Transform>(vfx).position = pos;
    em.GetComponent<Renderer2D>(vfx).m_SortingLayer = 5;
    em.GetComponent<EntityType>(vfx).type = EntityCategory::E_VFX;
    em.GetComponent<Image>(vfx).m_Sprite.m_Key = image;
    em.GetComponent<Animator>(vfx).m_ControllerKey = animation;
  }
}
