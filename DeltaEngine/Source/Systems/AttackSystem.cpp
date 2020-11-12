#include "AttackSystem.h"

namespace DeltaEngine
{

    void AttackSystem::Update()
    {
        em.ForEach([&](EntityID& id, Attack& a,Image& im)
        {
            if (a.isAttacking)
            {
                AttackingEntities.push_back(id);
                a.isAttacking = false;
            }
        });

        for (auto& id : AttackingEntities)
        {
            RangedAttack(id);
        }

        AttackingEntities.clear();

    }
    void AttackSystem::LateUpdate()
    {

    }

    void AttackSystem::RangedAttack(EntityID& id)
    {
       std::cout << "Ranged Attack" << std::endl;
       Transform& t1 = em.GetComponent<Transform>(id);
       EntityID missle = em.CreateEntity<Collider, Lifespan, Transform, RigidBody>();
       em.GetComponent<Transform>(missle).position = t1.position;
       em.GetComponent<RigidBody>(missle).Mass = 5.0f;
       em.GetComponent<Transform>(missle).scale = { 0.2,0.2,0.0 };
       em.GetComponent<Lifespan>(missle).Lifespan = 0.35f;
       em.GetComponent<RigidBody>(missle).hasGravity = true;
       em.GetComponent<Collider>(missle).isTrigger = true;

       em.GetComponent<RigidBody>(missle).FrictionCoeff = 0.0f;
       em.GetComponent<RigidBody>(missle).Movespeed = 40.0f;

       if (em.GetComponent<Image>(id).m_FlipX == false)
       {
           em.GetComponent<Transform>(missle).position.x += 0.8f;
           em.GetComponent<RigidBody>(missle).AccumulatedForce = { 7500,4000 };
       }
       else
       {
           em.GetComponent<Transform>(missle).position.x -= 0.8f;
           em.GetComponent<RigidBody>(missle).AccumulatedForce = { -7500,4000 };
       }



    }
}
