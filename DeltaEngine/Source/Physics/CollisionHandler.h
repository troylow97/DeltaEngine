#pragma once
#include "DEpch.h"
#include "Core/TypeAlias.h"
#include "Core/Containers/ActionList.h"
#include "OECS/Types.h"
//#include <unordered_map>

//Every object has a collider and a collision handler
//Add callbacks by using RegisterOnEnter API

namespace DeltaEngine
{
  using CollisionCallbacks = std::map<std::string, Action<>>;
  //std::unordered_map<std::string, Actions>> callback_table;
  class CollisionHandler //think of this as a functor
  {
    // Possible to do 1 for Collision & 1 for Trigger Checking
    Util::ActionList<Entity> _enter;
    Util::ActionList<Entity> _stay;
    Util::ActionList<Entity> _exit;

    void OnEnter(Entity col);
    void OnStay(Entity col);
    void OnExit(Entity col);

  public:
    friend class CollisionSystem;
    CollisionHandler() = default;

    static CollisionCallbacks m_callbacks;
    static std::map<std::string, CollisionCallbacks> m_handlers;

    size_t RegisterOnEnter(Action<Entity> action);
    size_t RegisterOnStay(Action<Entity> action);
    size_t RegisterOnExit(Action<Entity> action);

    void UnregisterOnEnter(size_t id);
    void UnregisterOnStay(size_t id);
    void UnregisterOnExit(size_t id);

  };

  //class CollisionHandlerSystem
  //{
  //public:
  //    std::map<ComponentGroup, CollisionHandler> CollisionHandling;
  //};

} // namespace DeltaEngine

 //Handler is just a callback(void function),
 //u pass parameters to it then it will do the respective function calls
 //Handler has its own handle ID (integer/string)
 //
 //eg -> std::map<std::string/integer, void function> m_handlers;