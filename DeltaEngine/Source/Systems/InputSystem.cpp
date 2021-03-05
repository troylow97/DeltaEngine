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

  void InputSystem::SetIdleAnimation()
  {
    for (size_t i = 0; i < GetEnv().pClock->Timesteps(); i++)
    {
      idle_timer += static_cast<float>(FixedDeltaTime());
      melee_attack_cooldown += static_cast<float>(FixedDeltaTime());
    }

    env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& id, RigidBody& r1, Input& i, State& a)
    {
      a.SetFloat("IsStretching", idle_timer);
      
      if (melee_attack_cooldown > 0.5f)
      {
        a.SetBool("MeleeAttack", false);
      }
      
      if (range_attack_cooldown > 0.5f)
      {
        a.SetBool("RangeAttack", false);
      }
    });
  }

  void InputSystem::GodMode()
  {
    if (!god_mode)
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, Attack& a, Health& h, Player& p)
      {
        h.isInvulnerable = true;
        a.MeleeComboDamage *= 2;
        a.MeleeDamage *= 2;
        a.RangedDamage *= 2;
        a.SMGDamage *= 2;
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
        a.SMGDamage /= 2;
      });
      god_mode = false;
    }
  }

  void InputSystem::RunLeft()
  {
    env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, Player& p1, Input& i1, RigidBody& r1, State& s1, Attack& a1, Image& im1)
    {
      if (p1.AllowRuning == true)
      {
        if (/*!a1.Blocking && */ a1.MeleeCooldownTimer <= 0.0)
        {
          p1.IsRunning = true;
          i1.previousKey = DEVK_A;
          
          r1.Direction = Vector2::left();
          if (r1.InherentAcceleration < r1.MaxAcceleration)
            r1.InherentAcceleration++;

          s1.SetBool("IsRunning", true);
          s1.SetBool("MeleeAttack", false);
          s1.SetBool("Punch1", false);
          s1.SetBool("Punch2", false);
          s1.SetBool("Punch3", false);
          
          a1.MeleeAttack = false;
          a1.RangeAttack = false;
          a1.NumberOfCombos = 0;
          
          im1.m_FlipX = true;
          idle_timer = 0.0f;
        }
      }
    });
  }
  
  void InputSystem::RunRight()
  {
    env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, Player& p1, Input& i1, RigidBody& r1, State& s1, Attack& a1, Image& im1)
    {
      if (p1.AllowRuning == true)
      {
        if (/*!a1.Blocking && */ a1.MeleeCooldownTimer <= 0.0) //TO edit punching
        {
          p1.IsRunning = true;
          i1.previousKey = DEVK_D;
          
          r1.Direction = Vector2::right();
          if (r1.InherentAcceleration < r1.MaxAcceleration)
            r1.InherentAcceleration++;

          s1.SetBool("IsRunning", true);
          s1.SetBool("MeleeAttack", false);
          s1.SetBool("Punch1", false);
          s1.SetBool("Punch2", false);
          s1.SetBool("Punch3", false);
          
          a1.MeleeAttack = false;
          a1.RangeAttack = false;
          a1.NumberOfCombos = 0;
          
          im1.m_FlipX = false;
          idle_timer = 0.0f;
        }
      }
    });
  }

  void InputSystem::StopRun()
  {
    env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, Player& p1, RigidBody& r1, State& s1, Attack& a1)
    {
      p1.IsRunning = false;
      r1.Direction = Vector2::zero();
      s1.SetBool("IsRunning", false);
      p1.AllowShooting = true;
      a1.NumberOfCombos = 0;
    });
  }
  
  void InputSystem::Jump()
  {
    env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, Player& p1, Input& i1, Collider& c1, State& s1, Attack& a1)
    {
      i1.previousKey = DEVK_W;
      if (c1.isCollidingOnFloor)
        p1.IsJumping = true;
      p1.IsRunning = false;
    
      if (a1.Blocking) // if shield is on, deactivate shield
      {
        s1.SetBool("ShieldUp", false);
        a1.Blocking = false;
      }
      a1.NumberOfCombos = 0;
      p1.AllowShooting = false;
      p1.AllowPunching = false;
      s1.SetBool("IsIdle", false);
      s1.SetBool("JumpStart", true);
      idle_timer = 0.0f;
    
      AudioEngine::Play2DEvent( "event:/Player/PlayerJump" );
    });
  }

  void InputSystem::Dash()
  {
    //env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, Player& p1, Input& i1, Transform& t1, Collider& c1, State& s1, Attack& a1, Image& im1)
    //{
    //  if ((p1.IsShooting == true && p1.IsPunching == true) || p1.IsShooting == true || p1.IsPunching == true)
    //  {
    //      p1.AllowShooting = false;
    //      p1.IsShooting = false;
    //      p1.AllowPunching = false;
    //      p1.IsPunching = false;
    //  }
    //  if (p1.IsShooting == false || p1.AllowShooting == false && p1.IsShooting == false && p1.IsPunching == false)
    //  {
    //    if (c1.isCollidingOnFloor && p1.AllowDashing)
    //    {
    //      p1.IsDashing = true;
    //      p1.AllowDashing = false;
    //      
    //      i1.previousKey = DEVK_Q;
    //      
    //      if (a1.Blocking)
    //      {
    //        s1.SetBool("ShieldUp", false);
    //        //s1.SetBool("isJumping", true);
    //        a1.Blocking = false;
    //      }
    //      
    //      s1.SetBool("LancerAttack", true);
    //      const EntityID missile = em.CreateEntity<Collider, Lifespan, RigidBody, Health>();
    //      em.GetComponent<Transform>(missile).position = t1.position;
    //      em.GetComponent<RigidBody>(missile).Mass = 5.0f;
    //      em.GetComponent<Transform>(missile).scale = { 0.4f, 0.4f, 0.0f };
    //      em.GetComponent<Lifespan>(missile).Timer = 0.35f;
    //      em.GetComponent<Collider>(missile).isTrigger = true;
    //      em.GetComponent<EntityType>(missile).type = EntityCategory::E_PLAYER_DASH;
    //      em.GetComponent<RigidBody>(missile).FrictionCoeff = 0.0f;
    //      em.GetComponent<Health>(missile).CurrentHealth = 1;
    //      
    //      if (im1.m_FlipX)
    //        p1.DashDirectionRight = false;
    //      else
    //        p1.DashDirectionRight = true;
    //      
    //      idle_timer = 0.0f;
    //    }
    //  }
    //});
  }

  void InputSystem::Dodge()
  {  	
    env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, Player& p1, Input& i1, Collider& c1, State& s1, Attack& a1, Image& im1)
    {
	  if(p1.IsRunning)
	  {
        StopRun();
	  }
    	
      //if ((p1.IsShooting == true && p1.IsPunching == true) || p1.IsShooting == true || p1.IsPunching == true)
      {
        p1.AllowShooting = false;
        p1.AllowPunching = false;
      }
      if (!p1.IsShooting)
      {
        //if (c1.isCollidingOnFloor && )
        {
          p1.IsDodging = true;
          p1.AllowDashing = false;
          
          i1.previousKey = DEVK_RBUTTON;
          
          //if (a1.Blocking)
          //{
          //  s1.SetBool("ShieldUp", false);
          //  //s1.SetBool("isJumping", true);
          //  a1.Blocking = false;
          //}
          s1.SetBool("IsDashing", true);

          a1.NumberOfCombos = 0;
          
          if (im1.m_FlipX)
            p1.DashDirectionRight = false;
          else
            p1.DashDirectionRight = true;
          
          idle_timer = 0.0f;
        }
      }
    });
  }   

  void InputSystem::Punch()
  {
    env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1,Collider& c1, Player& p1, Input& i1, State& s1, Attack& a1)
    {
      if (p1.IsRunning)
      {
        //p1.IsRunning = false;
        //p1.AllowRuning = false;
        return;
      }
    	
      if (p1.AllowPunching && c1.isCollidingOnFloor && !p1.IsShooting)
      {
        p1.IsPunching = true;
        i1.previousKey = DEVK_LBUTTON;
        
        if (a1.Blocking)
        {
          s1.SetBool("ShieldUp", false);
          //s1.SetBool("isJumping", true);
          a1.Blocking = false;
        }
        
        a1.MeleeAttack = true;
        idle_timer = 0.0f;
        range_attack_cooldown = 0.0f;
      }
    });
  }

  void InputSystem::Shield()
  {
    env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, Player& p1, Input& i1, State& s1, Attack& a1)
    {
      i1.previousKey = DEVK_LSHIFT;
      if (a1.Blocking == true) // unshielding
      {
        p1.IsBlocking = false;
        p1.AllowDashing = true;
        p1.AllowPunching = true;
        a1.Blocking = false;
        a1.NumberOfCombos = 0;
        p1.AllowShooting = true;
        s1.SetBool("ShieldUp", false);
      }
      else // shielding
      {
        p1.IsBlocking = true;
        p1.AllowDashing = false;
        p1.AllowPunching = false;
        a1.Blocking = true;
        a1.NumberOfCombos = 0;
        p1.AllowShooting = false;
        s1.SetBool("ShieldUp", true);
      }
    });
  }

  void InputSystem::Shoot()
  {
    env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, Player& p1, Input& i1, State& s1, Attack& a1)
    {
      if (p1.IsRunning)
      {
        p1.AllowRuning = false;
        p1.IsRunning = false;
        StopRun();
      }
      if (p1.AllowShooting)
      {
        a1.SMGAttack = true;
        a1.NumberOfCombos = 0;
        p1.IsShooting = true;
        i1.previousKey = DEVK_E;
        s1.SetBool("SMGAttack", true);
        idle_timer = 0.0f; // what's this troy low yee?
        p1.AllowPunching = false;
        p1.AllowJumping = false;
        p1.AllowDashing = false;
      }
    });
  }

  void InputSystem::Update()
  {
    // setting the animation for when the player does not have any input
    SetIdleAnimation();

    // god mode 
    if (InputManager::Instance().IsKeyTriggered(DEVK_0))
    {
      GodMode();
    }
    
    // moving left 
    if (InputManager::Instance().IsKeyPressed(DEVK_A))
    {
      RunLeft();
    }
    else if (InputManager::Instance().IsKeyReleased(DEVK_A))
    {
      StopRun();
    }

    // moving right
    if (InputManager::Instance().IsKeyPressed(DEVK_D))
    {
      RunRight();
    }
    else if (InputManager::Instance().IsKeyReleased(DEVK_D))
    {
      StopRun();
    }

    // jumping 
    if (InputManager::Instance().IsKeyTriggered(DEVK_W) || InputManager::Instance().IsKeyTriggered(DEVK_SPACE))
    {
      Jump();
    }

    // dashing
    if (InputManager::Instance().IsKeyTriggered(DEVK_Q)) 
    {
      Dash();
    }
    env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, Player& p1, Input& i1, State& s1)
    {
      if (p1.IsDashing == false)
        s1.SetBool("LancerAttack", false);
    });
    // dodging
    if (InputManager::Instance().IsKeyTriggered(DEVK_RBUTTON))
    {
      Dodge();
    }
    env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, Player& p1, Input& i1, State& s1)
    {
      if (p1.IsDodging == false)
        s1.SetBool("IsDashing", false);
    });

    //if (InputManager::Instance().IsKeyTriggered(DEVK_Z))
    //{
    //  env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, Input& i1, Attack& a1, Image& im, State& a)
    //  {
    //    a1.RangeAttack = true;
    //    idle_timer = 0.0f;
    //    melee_attack_cooldown = 0.0f;
    //  });
    //}

    // punching
    if (InputManager::Instance().IsKeyTriggered(DEVK_LBUTTON))
    {
      Punch();
    }
    else if (InputManager::Instance().IsKeyReleased(DEVK_LBUTTON))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, Input& i1, Player& p1)
      {
        p1.IsPunching = false;
      });
    }
    // shield up and down
    if (InputManager::Instance().IsKeyTriggered(DEVK_LSHIFT))
    {
      Shield();
    }
    
    // shooting
    if (InputManager::Instance().IsKeyPressed(DEVK_E))
    {
      Shoot();
    }
    else if (InputManager::Instance().IsKeyReleased(DEVK_E))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, Player& p1, Input& i1, State& s1, Attack& a1)
      {
        p1.IsShooting = false;
        p1.AllowRuning = true;
        p1.AllowPunching = true;
        p1.AllowJumping = true;
        s1.SetBool("SMGAttack", false);
        a1.SMGAttack = false;
        //idle_timer = 0.0f;
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
