#include "InputSystem.h"
#include "Components/Character.h"
#include "Input/InputManager.h"
#include "Components/Attack.h"
#include "Core/GlobalStruct.h"
#include "Core/Debugging/Profiler/Profiler.h"
#include "Input/Keys.h"

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
  float attack_cooldown{0.0f};

  void InputSystem::Update()
  {
    for (size_t i = 0; i < GetEnv().pClock->Timesteps(); i++)
    {
      idle_timer += static_cast<float>(FixedDeltaTime());
      attack_cooldown += static_cast<float>(FixedDeltaTime());
    }

    env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& id, RigidBody& r1, Input& i, State& a)
    {
      a.SetFloat("IsIdle", idle_timer);
      //if (abs(r1.Velocity.y) > 1.0f )
      //a.SetFloat( "PlayerVelocityY", r1.Velocity.y  );
      //if (!r1.isJumping &&  abs(r1.Velocity.y) > 1.0f)
      //  a.SetFloat( "PlayerVelocityY", -r1.Velocity.y );
      if (attack_cooldown > 0.5f)
      {
        a.SetBool("Punch", false);
      }

      a.SetBool("Ranged", false);
    });

    if (InputManager::Instance().IsKeyPressed(DEVK_LEFT))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, RigidBody& r1, Input& i1, State& a, Image& i)
      {
        i1.previousKey = DEVK_A;
        r1.Direction = Vector2::left();
        if (r1.InherentAcceleration < r1.MaxAcceleration)
          r1.InherentAcceleration++;
        a.SetBool("IsRunning", true);
        idle_timer = 0.0f;
        i.m_FlipX = true;
      });
    }
    else if (InputManager::Instance().IsKeyReleased(DEVK_LEFT))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, RigidBody& r1, Input& i1, State& a)
      {
        a.SetBool("IsRunning", false);
      });
    }
    if (InputManager::Instance().IsKeyPressed(DEVK_RIGHT))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, RigidBody& r1, Input& i1, State& a, Image& i)
      {
        i1.previousKey = DEVK_D;
        r1.Direction = Vector2::right();
        if (r1.InherentAcceleration < r1.MaxAcceleration)
          r1.InherentAcceleration++;

        a.SetBool("IsRunning", true);
        idle_timer = 0.0f;
        i.m_FlipX = false;
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
    if (InputManager::Instance().IsKeyPressed(DEVK_UP))
    {
        env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, RigidBody& r1, Input& i1, State& a, Image& i)
            {
                r1.Direction = Vector2::up();
                if (r1.InherentAcceleration < r1.MaxAcceleration)
                    r1.InherentAcceleration++;
            });
    }
    else if (InputManager::Instance().IsKeyReleased(DEVK_UP))
    {
        env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, RigidBody& r1, Input& i1, State& a, Image& i)
            {
                r1.Direction = Vector2::zero();
                if (r1.InherentAcceleration < r1.MaxAcceleration)
                    r1.InherentAcceleration++;
            });
    }

    if (InputManager::Instance().IsKeyPressed(DEVK_DOWN))
    {
        env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, RigidBody& r1, Input& i1, State& a, Image& i)
            {
                r1.Direction = Vector2::down();
                if (r1.InherentAcceleration < r1.MaxAcceleration)
                    r1.InherentAcceleration++;
            });
    }
    else if (InputManager::Instance().IsKeyReleased(DEVK_DOWN))
    {
        env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, RigidBody& r1, Input& i1, State& a, Image& i)
            {
                r1.Direction = Vector2::zero();
                if (r1.InherentAcceleration < r1.MaxAcceleration)
                    r1.InherentAcceleration++;
            });
    }
    //END TESTING-------------------------------------------------------------------------------------------------------


    if (InputManager::Instance().IsKeyTriggered(DEVK_SPACE))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, RigidBody& r1, Collider& c1, Input& i1)
      {
        if (c1.isCollidingOnFloor)
        {
          r1.isJumping = true;
        }
        i1.previousKey = DEVK_SPACE;
        idle_timer = 0.0f;
      });
    }

    if (InputManager::Instance().IsKeyReleased(DEVK_SPACE))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, RigidBody& r1, Input& i1)
      {
        i1.previousKey = DEVK_SPACE;
        r1.isJumping = false;
      });
    }

    if (InputManager::Instance().IsKeyTriggered(DEVK_C))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, Input& i1, Attack& a1, Image& im, State& a)
      {
        a1.RangeAttack = true;
        a.SetBool("Ranged", true);
        idle_timer = 0.0f;
        attack_cooldown = 0.0f;
      });
    }

    if (InputManager::Instance().IsKeyTriggered(DEVK_X))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, Input& i1, Attack& a1, Image& im, State& a)
      {
        a1.MeleeAttack = true;
        a.SetBool("Punch", true);
        idle_timer = 0.0f;
        attack_cooldown = 0.0f;
      });
    }

    if (InputManager::Instance().IsKeyReleased(DEVK_UP) || InputManager::Instance().IsKeyReleased(DEVK_DOWN)
      || InputManager::Instance().IsKeyReleased(DEVK_LEFT) || InputManager::Instance().IsKeyReleased(DEVK_RIGHT))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, RigidBody& r1, Input& i1)
      {
        r1.Direction = Vector2::zero();
        r1.InherentAcceleration = 0.0f;
      });
    }


    if (InputManager::Instance().IsKeyTriggered(DEVK_BACKSLASH)) // '\'
      if (InputManager::Instance().GetShowLine() == false)
        InputManager::Instance().SetShowLine(true);
      else if (InputManager::Instance().GetShowLine() == true)
        InputManager::Instance().SetShowLine(false);

    // gets the coordinates of the mouse, good for debugging
    if (InputManager::Instance().OnMouseMove())
    {
      auto curr = InputManager::Instance().CurrentPosition();
      auto prev = InputManager::Instance().PreviousPosition();
      //if ( curr != prev )
      //  DeltaEngine_CORE_TRACE( "Mouse Position: x({}) y({})", curr.point_x, curr.point_y );
    }

    if (InputManager::Instance().IsMouseTriggered(DEVK_LBUTTON))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& id1, Collider& c1, Transform& t1, RigidBody& r1)
      {
        if (c1.type == ColliderType::BOX)
        {
          if (CollisionIntersection_RectMouse(c1.center, c1.size, InputManager::Instance().CurrentCameraPosition()))
          {
            InputManager::Instance().SetEntitySelected(true);
            InputManager::Instance().SetEntityIDSelected(id1.index);
          }
        }
        else if (c1.type == ColliderType::CIRCLE)
        {
          if (CollisionIntersection_RectMouse(c1.center, c1.size, InputManager::Instance().CurrentCameraPosition()))
          {
            InputManager::Instance().SetEntitySelected(true);
            InputManager::Instance().SetEntityIDSelected(id1.index);
            //std::cout << "entity selected is " << InputManager::Instance().EntityIDSelected() << std::endl;
          }
        }
      });
    }

    if (InputManager::Instance().IsKeyPressed(DEVK_LCTRL) &&
      InputManager::Instance().IsKeyReleased(DEVK_RETURN))
      Profiler::Instance().Print();

    Profiler::Instance().Record("Input System");
  }

  void InputSystem::LateUpdate()
  {
  }
}
