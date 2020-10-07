#include "PhysicsDrawSystem.h"
#include "Physics/Collider.h"
#include "Core/Debugging/Gizmos.h"

namespace DeltaEngine
{

    void PhysicsDrawSystem::update()
    {
        em.for_each(e_query, [&](EntityID id, Collider& c, Transform& t)
            {
                switch (c.type)
                {
                case ColliderType::BOX:
                    Gizmos::Draw2DWireBox(t);
                    break;
                case ColliderType::CIRCLE:
                    Gizmos::Draw2DCircle(t);
                    break;
                case ColliderType::RAY:
                    Gizmos::Draw2DWireBox(t); //TO CHANGE
                    break;
                case ColliderType::LINE:
                    Gizmos::Draw2DWireBox(t); //TO CHANGE
                    break;
                }
            });
    }
    void PhysicsDrawSystem::late_update()
    {

    }
}
