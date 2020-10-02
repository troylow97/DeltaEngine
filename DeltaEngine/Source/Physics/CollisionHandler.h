/*
#pragma once
#include "Core/Containers/ActionList.h"
#include "Physics/CollisionModule.h"
#include <unordered_map>

 //Handler is just a callback(void function),
 //u pass parameters to it then it will do the respective function calls
 //Handler has its own handle ID (integer/string)
 //
 //eg -> std::map<std::string/integer, void function> m_handlers;

namespace DeltaEngine
{
  //std::unordered_map<std::string, Actions>> callback_table;
  class CollisionHandler //think of this as a functor
  {
    // Possible to do 1 for Collision & 1 for Trigger Checking
    ActionList<class Collider&> _enter;
    ActionList<class Collider&> _stay;
    ActionList<class Collider&> _exit;

    void OnEnter( class Collider &col);
    void OnStay( class Collider &col);
    void OnExit( class Collider &col);

    friend class CollisionModule; //Update, late update, 2 containers one to store previous frame one to store current.

  public:
    CollisionHandler() = default;

    size_t RegisterOnEnter(Action<class Collider &> &action);
    size_t RegisterOnStay(Action<class Collider &> &action);
    size_t RegisterOnExit(Action<class Collider &> &action);

    void UnregisterOnEnter(size_t id);
    void UnregisterOnStay(size_t id);
    void UnregisterOnExit(size_t id);

  };
  */
  //void t(int);
  //struct knock
  //{
  //    static void knockbackCalc(collider&);
  //};
  //CollisionHandler ch;
  //ch.RegisteronEnter(knock.KnockbackCalc())

  //std::unordered_map<std::string, ActionList>>
  //std::unordered_map<std::string, Actions>> <- callback table "Knockback",Knockback()




} // namespace DeltaEngine
*/