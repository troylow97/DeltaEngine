#include "AttackSystem.h"

namespace DeltaEngine
{

    void AttackSystem::Update()
    {
        em.ForEach(e_query, [&](EntityID& id, Attack& a, Transform& t, RigidBody& r,EntityType& et)
        {
            if (a.isAttacking)
            {
                if (et.type == "monster")
                {
                    RangedAttack(id);
                }
                else if (et.type == "player")
                {

                }
                a.isAttacking = false;
            }
        });
    }
    void AttackSystem::LateUpdate()
    {

    }

    void AttackSystem::RangedAttack(EntityID& id)
    {

    }
}
