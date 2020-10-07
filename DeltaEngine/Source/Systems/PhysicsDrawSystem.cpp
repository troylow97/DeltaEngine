#include "PhysicsDrawSystem.h"
#include "Physics/Collider.h"
namespace DeltaEngine
{

    void PhysicsDrawSystem::update()
    {
        em.for_each(e_query, [&](EntityID id, Collider& c)
            {
                OnDrawGizmos(c);
            });
    }
    void PhysicsDrawSystem::late_update()
    {

    }
}
