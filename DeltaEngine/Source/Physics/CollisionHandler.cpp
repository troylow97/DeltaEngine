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
