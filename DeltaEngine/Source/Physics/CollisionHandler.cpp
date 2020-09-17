#include "CollisionHandler.h"

namespace DeltaEngine
{

  void CollisionHandler::OnEnter(Collider &col)
  {
    _enter.Invoke(col);
  }
  void CollisionHandler::OnStay(Collider &col)
  {
    _stay.Invoke(col);
  }
  void CollisionHandler::OnExit(Collider &col)
  {
    _exit.Invoke(col);
  }

  size_t CollisionHandler::RegisterOnEnter(Action<class Collider &> &action)
  {
    return _enter.Subscribe(action);
  }
  size_t CollisionHandler::RegisterOnStay(Action<class Collider &> &action)
  {
    return _stay.Subscribe(action);
  }
  size_t CollisionHandler::RegisterOnExit(Action<class Collider &> &action)
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
