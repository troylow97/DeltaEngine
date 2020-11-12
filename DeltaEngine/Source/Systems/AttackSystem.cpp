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
       EntityID missile = em.CreateEntity<Collider, Lifespan, Transform, RigidBody,EntityType>();
       em.GetComponent<Transform>(missile).position = t1.position;
       em.GetComponent<RigidBody>(missile).Mass = 5.0f;
       em.GetComponent<Transform>(missile).scale = { 0.2,0.2,0.0 };
       em.GetComponent<Lifespan>(missile).Timer = 0.35f;
       em.GetComponent<RigidBody>(missile).hasGravity = true;
       em.GetComponent<Collider>(missile).isTrigger = true;
       em.GetComponent<EntityType>(missile).type = entity_type::E_BULLET;
       em.GetComponent<RigidBody>(missile).FrictionCoeff = 0.0f;
       em.GetComponent<RigidBody>(missile).Movespeed = 40.0f;

       if (em.GetComponent<Image>(id).m_FlipX == false)
       {
           em.GetComponent<Transform>(missile).position.x += 0.8f;
           em.GetComponent<RigidBody>(missile).AccumulatedForce = { 7500,4000 };
       }
       else
       {
           em.GetComponent<Transform>(missile).position.x -= 0.8f;
           em.GetComponent<RigidBody>(missile).AccumulatedForce = { -7500,4000 };
       }



    }
}
