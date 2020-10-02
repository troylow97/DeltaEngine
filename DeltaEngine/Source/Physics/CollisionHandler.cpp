#include "CollisionHandler.h"

namespace DeltaEngine
{
    std::map<std::string, CollisionCallbacks> CollisionHandler::m_handlers; //m_handlers["ReduceHP"] = &ReduceHP;

  void CollisionHandler::OnEnter(Entity col)
  {
    _enter.Invoke(col);
  }
  void CollisionHandler::OnStay(Entity col)
  {
    _stay.Invoke(col);
  }
  void CollisionHandler::OnExit(Entity col)
  {

    _exit.Invoke(col);
  }

  size_t CollisionHandler::RegisterOnEnter(Action<Entity> action)
  {
    return _enter.Subscribe(action);
  }
  size_t CollisionHandler::RegisterOnStay(Action<Entity> action)
  {
    return _stay.Subscribe(action);
  }
  size_t CollisionHandler::RegisterOnExit(Action<Entity> action)
  {
    return _exit.Subscribe(action);
  }

  void CollisionHandler::UnregisterOnEnter(size_t id)
  {
    _enter.Unsubscribe(id);
  }
  void CollisionHandler::UnregisterOnStay(size_t id)
  {
    _stay.Unsubscribe(id);
  }
  void CollisionHandler::UnregisterOnExit(size_t id)
  {
    _exit.Unsubscribe(id);
  }

} // namespace DeltaEngine
