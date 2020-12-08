/**********************************************************************************
* \file   CollisionHandler.cpp
* \brief  The file contains BLAHBLAHBLAH
* \author Chin, Clara,   X% Code Contribution
* \author Low, Troy,     X% Code Contribution
* \author Ong, Graeme,   X% Code Contribution
* \author Tan, Tong Wee, X% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "CollisionHandler.h"

namespace DeltaEngine
{
  void CollisionHandler::OnEnter(EntityID& id1, EntityID& id2)
  {
    m_enter.Invoke(id1, id2);
  }

  void CollisionHandler::OnStay(EntityID& id1, EntityID& id2)
  {
    m_stay.Invoke(id1, id2);
  }

  void CollisionHandler::OnExit(EntityID& id1, EntityID& id2)
  {
    m_exit.Invoke(id1, id2);
  }

  size_t CollisionHandler::RegisterOnEnter(Action<EntityID&, EntityID&> action)
  {
    return m_enter.Subscribe(action);
  }

  size_t CollisionHandler::RegisterOnStay(Action<EntityID&, EntityID&> action)
  {
    return m_stay.Subscribe(action);
  }

  size_t CollisionHandler::RegisterOnExit(Action<EntityID&, EntityID&> action)
  {
    return m_exit.Subscribe(action);
  }

  void CollisionHandler::UnregisterOnEnter(size_t id)
  {
    m_enter.Unsubscribe(id);
  }

  void CollisionHandler::UnregisterOnStay(size_t id)
  {
    m_stay.Unsubscribe(id);
  }

  void CollisionHandler::UnregisterOnExit(size_t id)
  {
    m_exit.Unsubscribe(id);
  }
} // namespace DeltaEngine
