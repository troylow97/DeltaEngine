#include "AttackSystem.h"

namespace DeltaEngine
{

    void AttackSystem::Update()
    {
        em.ForEach(e_query, [&](EntityID id, Attack& a, Transform& t, RigidBody& r,EntityType et)
        {
            if (a.isAttacking)
            {
                

            }
        });
    }
    void AttackSystem::LateUpdate()
    {

    }
}
