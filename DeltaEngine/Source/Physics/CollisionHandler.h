/**********************************************************************************
* \file   CollisionHandler.h
* \brief  The file contains the logic for collision handling
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "Core/TypeAlias.h"
#include "Core/Containers/ActionList.h"
#include "ECS/Entities.h"

namespace DeltaEngine
{
  class CollisionHandler
  {
    Util::ActionList<EntityID&, EntityID&> m_enter;
    Util::ActionList<EntityID&, EntityID&> m_stay;
    Util::ActionList<EntityID&, EntityID&> m_exit;

    void OnEnter(EntityID&, EntityID&);
    void OnStay(EntityID&, EntityID&);
    void OnExit(EntityID&, EntityID&);
    friend struct CollisionSystem;
  public:
    CollisionHandler() = default;


    size_t RegisterOnEnter(Action<EntityID&, EntityID&> action);
    size_t RegisterOnStay(Action<EntityID&, EntityID&> action);
    size_t RegisterOnExit(Action<EntityID&, EntityID&> action);

    void UnregisterOnEnter(size_t id);
    void UnregisterOnStay(size_t id);
    void UnregisterOnExit(size_t id);
  };
}
