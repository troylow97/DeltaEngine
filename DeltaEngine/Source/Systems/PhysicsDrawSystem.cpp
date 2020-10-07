#include "PhysicsDrawSystem.h"

namespace DeltaEngine
{

    void PhysicsDrawSystem::update()
    {
        em.for_each(e_query, [&](EntityID id, Collider& c)
            {
                c.OnDrawGizmos();
            });
    }
    void PhysicsDrawSystem::late_update()
    {

    }
}
