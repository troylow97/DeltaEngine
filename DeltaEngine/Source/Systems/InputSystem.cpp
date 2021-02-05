/**********************************************************************************
* \file   InputSystem.cpp
* \brief  The file contains implementation of InputSystem.
* \author Chin, Clara,   50% Code Contribution
* \author Low, Troy,     50% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "InputSystem.h"

#include "PhysicsDrawSystem.h"
#include "Components/Character.h"
#include "Input/InputManager.h"
#include "Components/Attack.h"
#include "Core/GlobalStruct.h"
#include "Core/Debugging/Profiler/Profiler.h"
#include "Input/Keys.h"
#include "../../Sandbox/Source/Systems/UnitManager.h"
#include "Audio/AudioEngine.h"

namespace DeltaEngine
{
  void InputSystem::Initialize()
  {
    DeltaEngine_CORE_TRACE("Initialize Input System");
  }

  void InputSystem::Shutdown()
  {
    DeltaEngine_CORE_TRACE("Shutdown Input System");
  }

  float idle_timer{0.0f};
  float melee_attack_cooldown{0.0f};
  float range_attack_cooldown{0.0f};
  bool god_mode = false;

  void InputSystem::Update()
  {
    for (size_t i = 0; i < GetEnv().pClock->Timesteps(); i++)
    {
      idle_timer += static_cast<float>(FixedDeltaTime());
      melee_attack_cooldown += static_cast<float>(FixedDeltaTime());
    }

    env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& id, RigidBody& r1, Input& i, State& a)
    {
      a.SetFloat("IsIdle", idle_timer);

      //if (melee_attack_cooldown > 0.5f )
      //{
      //  a.SetBool( "MeleeAttack", false );
      //}
      //
      //if (range_attack_cooldown > 0.5f)
      //{
      //    a.SetBool("RangeAttack", false);
      //}
    });

    if (InputManager::Instance().IsKeyTriggered(DEVK_0))
    {
      if (!god_mode)
      {
        env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, Attack& a, Health& h, Player& p)
        {
          h.isInvulnerable = true;
          a.MeleeComboDamage *= 2;
          a.MeleeDamage *= 2;
          a.RangedDamage *= 2;
          a.SMGAttack *= 2;
        });
        god_mode = true;
      }
      else
      {
        env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, Attack& a, Health& h, Player& p)
        {
          h.isInvulnerable = false;
          a.MeleeComboDamage /= 2;
          a.MeleeDamage /= 2;
          a.RangedDamage /= 2;
          a.SMGAttack /= 2;
        });
        god_mode = false;
      }
    }

    if (InputManager::Instance().IsKeyPressed(DEVK_LEFT) && !InputManager::Instance().IsKeyPressed(DEVK_C))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach(
        [&](EntityID id1, RigidBody& r1, Input& i1, State& a, Image& i, Attack& att)
        {
          if (att.MeleeCooldownTimer <= 0.0)
          {
            i1.previousKey = DEVK_A;
            r1.Direction = Vector2::left();
            if (r1.InherentAcceleration < r1.MaxAcceleration)
              r1.InherentAcceleration++;
            a.SetBool("IsRunning", true);
            a.SetBool("MeleeAttack", false);
            a.SetBool("Punch1", false);
            a.SetBool("Punch2", false);
            a.SetBool("Punch3", false);
            att.MeleeAttack = false;
            att.RangeAttack = false;
            att.SMGAttack = false;

            idle_timer = 0.0f;
            i.m_FlipX = true;
          }
        });
    }
    else
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, RigidBody& r1, Input& i1, State& a)
      {
        a.SetBool("IsRunning", false);
      });
    }
    if (InputManager::Instance().IsKeyPressed(DEVK_RIGHT) && !InputManager::Instance().IsKeyPressed(DEVK_C))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach(
        [&](EntityID id1, RigidBody& r1, Input& i1, State& a, Image& i, Attack& att)
        {
          if (att.MeleeCooldownTimer <= 0.0)
          {
            i1.previousKey = DEVK_D;
            r1.Direction = Vector2::right();
            if (r1.InherentAcceleration < r1.MaxAcceleration)
              r1.InherentAcceleration++;

            a.SetBool("IsRunning", true);
            a.SetBool("MeleeAttack", false);
            a.SetBool("Punch1", false);
            a.SetBool("Punch2", false);
            a.SetBool("Punch3", false);
            att.MeleeAttack = false;
            att.RangeAttack = false;
            att.SMGAttack = false;
            idle_timer = 0.0f;
            i.m_FlipX = false;
          }
        });
    }
    else if (InputManager::Instance().IsKeyReleased(DEVK_RIGHT))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, RigidBody& r1, Input& i1, State& a)
      {
        a.SetBool("IsRunning", false);
      });
    }

    //FOR TESTING-------------------------------------------------------------------------------------------------------
    /*if ( InputManager::Instance().IsKeyPressed( DEVK_UP ) )
    {
      env.pECS->GetWorld().GetEntityManager().ForEach( [&]( EntityID id1, RigidBody &r1, Input &i1, State &a, Image &i )
      {
        r1.Direction = Vector2::up();
        if ( r1.InherentAcceleration < r1.MaxAcceleration )
          r1.InherentAcceleration++;
      } );
    }
    else if ( InputManager::Instance().IsKeyReleased( DEVK_UP ) )
    {
      env.pECS->GetWorld().GetEntityManager().ForEach( [&]( EntityID id1, RigidBody &r1, Input &i1, State &a, Image &i )
      {
        r1.Direction = Vector2::zero();
        if ( r1.InherentAcceleration < r1.MaxAcceleration )
          r1.InherentAcceleration++;
      } );
    }
    
    if ( InputManager::Instance().IsKeyPressed( DEVK_DOWN ) )
    {
      env.pECS->GetWorld().GetEntityManager().ForEach( [&]( EntityID id1, RigidBody &r1, Input &i1, State &a, Image &i )
      {
        r1.Direction = Vector2::down();
        if ( r1.InherentAcceleration < r1.MaxAcceleration )
          r1.InherentAcceleration++;
      } );
    }
    else if ( InputManager::Instance().IsKeyReleased( DEVK_DOWN ) )
    {
      env.pECS->GetWorld().GetEntityManager().ForEach( [&]( EntityID id1, RigidBody &r1, Input &i1, State &a, Image &i )
      {
        r1.Direction = Vector2::zero();
        if ( r1.InherentAcceleration < r1.MaxAcceleration )
          r1.InherentAcceleration++;
      } );
    }*/
    //END TESTING-------------------------------------------------------------------------------------------------------


    if (InputManager::Instance().IsKeyTriggered(DEVK_SPACE))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, State& a, Collider& c1, Player& p1, Input& i1)
      {
        if (c1.isCollidingOnFloor)
          p1.IsJumping = true;
        i1.previousKey = DEVK_SPACE;
        idle_timer = 0.0f;

        AudioEngine::Play("Audio/jump.wav");
      });
    }
    if (InputManager::Instance().IsKeyReleased(DEVK_SPACE))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, Player& p1, RigidBody& r1, Input& i1)
      {
        i1.previousKey = DEVK_SPACE;
        p1.IsJumping = false;
      });
    }

    if (InputManager::Instance().IsKeyTriggered(DEVK_X)) //DASH
    {
      env.pECS->GetWorld().GetEntityManager().ForEach(
        [&](EntityID id1, Transform& t1, RigidBody r1, State& s, Image& a, Collider& c1, Player& p1, Input& i1)
        {
          if (c1.isCollidingOnFloor && p1.AllowDashing)
          {
            if (a.m_FlipX)
              p1.DashDirectionRight = false;
            else
              p1.DashDirectionRight = true;
            p1.IsDashing = true;
            p1.AllowDashing = false;
            s.SetBool("LancerAttack", true);
            const EntityID missile = em.CreateEntity<Collider, Lifespan, RigidBody, Health>();
            em.GetComponent<Transform>(missile).position = t1.position;
            em.GetComponent<RigidBody>(missile).Mass = 5.0f;
            em.GetComponent<Transform>(missile).scale = {0.4f, 0.4f, 0.0f};
            em.GetComponent<Lifespan>(missile).Timer = 0.35f;
            em.GetComponent<Collider>(missile).isTrigger = true;
            em.GetComponent<EntityType>(missile).type = EntityCategory::E_PLAYER_DASH;
            em.GetComponent<RigidBody>(missile).FrictionCoeff = 0.0f;
            em.GetComponent<Health>(missile).CurrentHealth = 1;
          }
          i1.previousKey = DEVK_X;
        });
    }

    if (InputManager::Instance().IsKeyTriggered(DEVK_Z))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, Input& i1, Attack& a1, Image& im, State& a)
      {
        a1.RangeAttack = true;
        idle_timer = 0.0f;
        melee_attack_cooldown = 0.0f;
      });
    }

    if (InputManager::Instance().IsKeyTriggered(DEVK_C))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, Input& i1, Attack& a1, Image& im, State& a)
      {
        a1.MeleeAttack = true;
        idle_timer = 0.0f;
        range_attack_cooldown = 0.0f;
      });
    }

    if (InputManager::Instance().IsKeyPressed(DEVK_LBUTTON))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, Input& i1, Attack& a1, Image& im, State& a)
      {
        a1.SMGAttack = true;
        idle_timer = 0.0f; // what's this troy low yee?
      });
    }
    if (!(InputManager::Instance().IsKeyPressed(DEVK_LBUTTON)))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, Input& i1, Attack& a1, Image& im, State& a)
      {
        a1.SMGAttack = false;
        idle_timer = 0.0f; // what's this troy low yee?
      });
    }

    //if (InputManager::Instance().IsKeyTriggered(DEVK_COMMA))
    //{
    //  auto& p = env.pECS->GetWorld().GetEntityManager().GetComponent<Player>(UnitManager::GetPlayerID());
    //  p.UpgradeAtk = true;
    //}
    //if (InputManager::Instance().IsKeyTriggered(DEVK_PERIOD))
    //{
    //  auto& p = env.pECS->GetWorld().GetEntityManager().GetComponent<Player>(UnitManager::GetPlayerID());
    //  p.UpgradeHP = true;
    //}


    if (InputManager::Instance().IsKeyReleased(DEVK_UP) || InputManager::Instance().IsKeyReleased(DEVK_DOWN)
      || InputManager::Instance().IsKeyReleased(DEVK_LEFT) || InputManager::Instance().IsKeyReleased(DEVK_RIGHT))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, RigidBody& r1, Input& i1)
      {
        r1.Direction = Vector2::zero();
        r1.InherentAcceleration = 0.0f;
      });
    }

#ifdef DE_EDITOR
  if ( InputManager::Instance().IsKeyTriggered( DEVK_BACKSLASH ) ) // '\'
    PhysicsDrawSystem::gizmo = !PhysicsDrawSystem::gizmo;
#endif

    if (InputManager::Instance().IsKeyPressed(DEVK_LCTRL) &&
      InputManager::Instance().IsKeyReleased(DEVK_RETURN))
      Profiler::Instance().Print();

    Profiler::Instance().Record("Input System");
  }


  void InputSystem::LateUpdate()
  {
  }
}
