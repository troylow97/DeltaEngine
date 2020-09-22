/*
#pragma once
#include "ActionList.h"
#include <cstddef>

namespace DeltaEngine
{
  class CollisionHandler
  {
    // Possible to do 1 for Collision & 1 for Trigger Checking
    ActionList<class Collider &> _enter;
    ActionList<class Collider &> _stay;
    ActionList<class Collider &> _exit;

    void OnEnter( class Collider &col);
    void OnStay( class Collider &col);
    void OnExit( class Collider &col);

    friend class CollisionModule;

  public:
    CollisionHandler() = default;

    size_t RegisterOnEnter(Action<class Collider &> &action);
    size_t RegisterOnStay(Action<class Collider &> &action);
    size_t RegisterOnExit(Action<class Collider &> &action);

    void UnregisterOnEnter(size_t id);
    void UnregisterOnStay(size_t id);
    void UnregisterOnExit(size_t id);

  };

} // namespace DeltaEngine
*/