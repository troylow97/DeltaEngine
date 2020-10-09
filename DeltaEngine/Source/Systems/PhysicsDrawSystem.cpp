#include "PhysicsDrawSystem.h"
#include "ECS/Components/Collider.h"
#include "Core/Debugging/Gizmos.h"
#include "Input/InputManager.h"

namespace DeltaEngine
{

    void PhysicsDrawSystem::update()
    {
        em.for_each(e_query, [&](EntityID id, Collider& c, Transform& t, RigidBody& r)
            {
                switch (c.type)
                {
                case ColliderType::BOX:
                    if (InputManager::get()->getShowLine() == true)
                    {
                        Gizmos::DrawLine(t.position, t.position + r.Velocity);
                    }
                    Gizmos::Draw2DWireBox(t);
                    break;
                case ColliderType::CIRCLE:
                {
                    if (InputManager::get()->getShowLine() == true)
                    {
                        Gizmos::DrawLine(t.position, t.position + r.Velocity);
                    }
                    Transform temp = t;
                    temp.scale *= 2;
                    Gizmos::Draw2DCircle(temp);
                    break;
                }
                case ColliderType::RAY:
                    if (InputManager::get()->getShowLine() == true)
                    {
                        Gizmos::DrawLine(t.position, t.position + r.Velocity);
                    }
                    Gizmos::Draw2DWireBox(t); //TO CHANGE
                    break;
                case ColliderType::LINE:
                    if (InputManager::get()->getShowLine() == true)
                    {
                        Gizmos::DrawLine(t.position, t.position + r.Velocity);
                    }
                    Gizmos::Draw2DWireBox(t); //TO CHANGE
                    break;
                case ColliderType::NONE:
                    if (InputManager::get()->getShowLine() == true)
                    {
                        Gizmos::DrawLine(t.position, t.position + r.Velocity);
                    }
                    break;
                }
            });
    }
    void PhysicsDrawSystem::late_update()
    {

    }
}
