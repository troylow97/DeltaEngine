#include "PhysicsDrawSystem.h"
#include "Components/Collider.h"
#include "Core/Debugging/Gizmos.h"
#include "Core/Debugging/Profiler/Profiler.h"
#include "Input/InputManager.h"

namespace DeltaEngine
{
  void PhysicsDrawSystem::Update()
  {
    if (InputManager::Instance().GetShowLine() == true)

      em.ForEach(e_query, [&](EntityID id, Collider& c, Transform& t, RigidBody& r)
      {
        switch (c.type)
        {
        case ColliderType::BOX:
          if (InputManager::Instance().GetShowLine() == true)
          {
            Gizmos::DrawLine(t.position, t.position + r.Velocity);
          }
          Gizmos::Draw2DWireBox(t);
          break;
        case ColliderType::CIRCLE:
          {
            if (InputManager::Instance().GetShowLine() == true)
            {
              Gizmos::DrawLine(t.position, t.position + r.Velocity);
            }
            Transform temp = t;
            temp.scale *= 2;
            Gizmos::Draw2DCircle(temp);
            break;
          }
        case ColliderType::RAY:
          if (InputManager::Instance().GetShowLine() == true)
          {
            Gizmos::DrawLine(t.position, t.position + r.Velocity);
          }
          Gizmos::Draw2DWireBox(t); //TO CHANGE
          break;
        case ColliderType::LINE:
          if (InputManager::Instance().GetShowLine() == true)
          {
            Gizmos::DrawLine(t.position, t.position + r.Velocity);
          }
          Gizmos::Draw2DWireBox(t); //TO CHANGE
          break;
        case ColliderType::NONE:
          if (InputManager::Instance().GetShowLine() == true)
          {
            Gizmos::DrawLine(t.position, t.position + r.Velocity);
          }
          break;
        }
      });
    Profiler::Instance().Record("Physics Draw System");
  }

  void PhysicsDrawSystem::LateUpdate()
  {
  }
}
