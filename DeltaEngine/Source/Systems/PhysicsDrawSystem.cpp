#include "PhysicsDrawSystem.h"
#include "Components/Collider.h"
#include "Components/Dragbox.h"
#include "Core/Debugging/Gizmos.h"
#include "Core/Debugging/Profiler/Profiler.h"
#include "Input/InputManager.h"
#include "Core/GlobalStruct.h"
#include "Systems/InputSystem.cpp"
#include "ImGui/Panels/WorldPanel.cpp"

namespace DeltaEngine
{
  void PhysicsDrawSystem::Update()
  {
    if (InputManager::Instance().EntitySelected() && InputManager::Instance().EntityChosenInViewport()/* && InputManager::Instance().EntityAllowDrag()*/)
    {
        drag_box.drag_box_transform.scale = { 0.3f, 0.3f, 0.0f };
        Gizmos::Draw2DWireBox(drag_box.drag_box_transform);
    }
    else if (InputManager::Instance().EntitySelected() && InputManager::Instance().EntityChosenInWorld())
    {
      _drag_box.drag_box_transform.scale = { 0.3f, 0.3f, 0.0f };
      Gizmos::Draw2DWireBox(_drag_box.drag_box_transform);
    }

    if (InputManager::Instance().GetShowLine() == true)
    { 
      em.ForEach(e_query, [&](EntityID id, Collider& c, Transform& t, RigidBody& r)
      {
         Vector2 pos = c.center + c.offset;
        switch (c.type)
        {
        case ColliderType::BOX:
          if (InputManager::Instance().GetShowLine() == true)
          {
            Gizmos::DrawLine(pos, c.center + r.Velocity);
          }
          Gizmos::Draw2DWireBox(t,c.offset);
          break;
        case ColliderType::CIRCLE:
          {
            if (InputManager::Instance().GetShowLine() == true)
            {
              Gizmos::DrawLine(pos, c.center + r.Velocity);
            }
            Transform temp = t;
            temp.scale *= 2;
            Gizmos::Draw2DCircle(temp);
            break;
          }
        case ColliderType::RAY:
          if (InputManager::Instance().GetShowLine() == true)
          {
            Gizmos::DrawLine(pos, c.center + r.Velocity);
          }
          Gizmos::Draw2DWireBox(t); //TO CHANGE
          break;
        case ColliderType::LINE:
          if (InputManager::Instance().GetShowLine() == true)
          {
            Gizmos::DrawLine(pos, c.center + r.Velocity);
          }
          Gizmos::Draw2DWireBox(t); //TO CHANGE
          break;
        case ColliderType::NONE:
          if (InputManager::Instance().GetShowLine() == true)
          {
            Gizmos::DrawLine(c.center, c.center + r.Velocity);
          }
          break;
        }
      });
    }
    Profiler::Instance().Record("Physics Draw System");
  }

  void PhysicsDrawSystem::LateUpdate()
  {
  }
}
