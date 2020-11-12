#include "AttackSystem.h"

namespace DeltaEngine
{

    void AttackSystem::Update()
    {
        em.ForEach([&](EntityID& id, Attack& a)
        {
            if (a.isAttacking)
            {
                RangedAttack(id);
                //if (et.type == "monster")
                //{
                //    RangedAttack(id);
                //}
                //else if (et.type == "player")
                //{
                //
                //}
                a.isAttacking = false;
            }
        });
    }
    void AttackSystem::LateUpdate()
    {

    }

    void AttackSystem::RangedAttack(EntityID& id)
    {
        std::cout << "Ranged Attack" << std::endl;
        Transform& t1 = em.GetComponent<Transform>(id);
        EntityID missle = em.CreateEntity<Collider, Lifespan, Transform, RigidBody>();

        em.GetComponent<Transform>(missle).scale = { 0.2,0.2,0.0 };
        em.GetComponent<Lifespan>(missle).Lifespan = 3.0f;
        em.GetComponent<Transform>(missle).position = t1.position;
        em.GetComponent<Transform>(missle).position.x += 1.0f;
        em.GetComponent<RigidBody>(missle).Velocity.x += 1.0f;

    }
}
