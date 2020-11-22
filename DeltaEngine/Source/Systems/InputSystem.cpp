#include "InputSystem.h"
#include "Components/Character.h"
#include "Input/InputManager.h"
#include "Components/Attack.h"
#include "Components/Dragbox.h"
#include "Core/GlobalStruct.h"
#include "Core/Debugging/Profiler/Profiler.h"
#include "Input/Keys.h"
#include "Core/Debugging/Gizmos.h"

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
  Dragbox drag_box;

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
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, RigidBody& r1, Collider& c1, Input& i1, State& a)
      {
        if (c1.isCollidingOnFloor)
        {
            a.SetFloat("PlayerVelocityY", 1);
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

    SelectEntity();
    DragEntity();
    DragCamera();

    if (InputManager::Instance().IsKeyPressed(DEVK_LCTRL) &&
      InputManager::Instance().IsKeyReleased(DEVK_RETURN))
      Profiler::Instance().Print();

    Profiler::Instance().Record("Input System");
  }

  void InputSystem::SelectEntity()
  {
    if (InputManager::Instance().IsMouseTriggered(DEVK_LBUTTON))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& id1, Transform& t1, Image& i1)
      {
        if (CollisionIntersection_RectMouse(t1.position, i1.m_Size, InputManager::Instance().CurrentCameraPosition()))
        {
          InputManager::Instance().SetEntitySelected(true);
          InputManager::Instance().SetEntityIDSelected(id1.index);
          InputManager::Instance().SetCameraDragged(false);
          InputManager::Instance().SetEntityChosenInViewport(true);
          InputManager::Instance().SetEntityChosenInWorld(false);
          
          auto& t = em.GetComponent<Transform>({ InputManager::Instance().EntityIDSelected() });
          
          drag_box.drag_box_transform = t;
          drag_box.drag_box_transform.position = t.position;
          drag_box.drag_box_transform.scale = { 0.3f, 0.3f, 0.0f };
        }
      });
    }
  }

  void InputSystem::DragEntity()
  {
    if (InputManager::Instance().EntityAllowDrag() && InputManager::Instance().MouseInViewPort())
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& id1, Transform& t1, Image& i1)
      {
        if (CollisionIntersection_RectMouse(drag_box.drag_box_transform.position, drag_box.drag_box_transform.scale, InputManager::Instance().CurrentCameraPosition()))
        {
          InputManager::Instance().SetEntityDragged(true);
        }
        if (InputManager::Instance().EntityDragged())
        {
          if (InputManager::Instance().IsMousePressed(DEVK_LBUTTON) && InputManager::Instance().EntitySelected())
          {
            if (CollisionIntersection_RectMouse(t1.position, t1.scale, InputManager::Instance().CurrentCameraPosition()))
            {
              float x_transform = InputManager::Instance().CurrentCameraPosition().point_x - InputManager::Instance().PreviousCameraPosition().point_x;
              float y_transform = InputManager::Instance().CurrentCameraPosition().point_y - InputManager::Instance().PreviousCameraPosition().point_y;
          
              if (id1.index == InputManager::Instance().EntityIDSelected())
              {
                t1.position.x = InputManager::Instance().CurrentCameraPosition().point_x;
                t1.position.y = InputManager::Instance().CurrentCameraPosition().point_y;
                //t1.position.x += x_transform;
                //t1.position.y += y_transform;
              }
            }
          }
        }
      });
    }
    else if (InputManager::Instance().IsMouseReleased(DEVK_LBUTTON))
    {
      InputManager::Instance().SetEntityDragged(false);
    }
  }

  void InputSystem::DragCamera()
  {
    if (InputManager::Instance().IsKeyPressed(DEVK_W))
    {
      Camera::editorCamera->transform.position.y += 0.03f;
    }
    else if (InputManager::Instance().IsKeyPressed(DEVK_A))
    {
      Camera::editorCamera->transform.position.x -= 0.03f;
    }
    else if (InputManager::Instance().IsKeyPressed(DEVK_S))
    {
      Camera::editorCamera->transform.position.y -= 0.03f;
    }
    else if (InputManager::Instance().IsKeyPressed(DEVK_D))
    {
      Camera::editorCamera->transform.position.x += 0.03f;
    }
    else if (InputManager::Instance().IsKeyPressed(DEVK_Q))
    {
      Camera::editorCamera->m_Size -= 0.01f;
    }
    else if (InputManager::Instance().IsKeyPressed(DEVK_E))
    {
      Camera::editorCamera->m_Size += 0.01f;
    }
    
    if (InputManager::Instance().CameraDragged() && InputManager::Instance().MouseInViewPort())
    {
      if (InputManager::Instance().IsMousePressed(DEVK_LBUTTON))
      {
        float x_transform = InputManager::Instance().CurrentCameraPosition().point_x - InputManager::Instance().PreviousCameraPosition().point_x;
        float y_transform = InputManager::Instance().CurrentCameraPosition().point_y - InputManager::Instance().PreviousCameraPosition().point_y;
        
        Camera::editorCamera->transform.position.x -= x_transform * 0.5f;
        Camera::editorCamera->transform.position.y -= y_transform * 0.75f;
      }
    }
  }

  void InputSystem::LateUpdate()
  {
  }
}
