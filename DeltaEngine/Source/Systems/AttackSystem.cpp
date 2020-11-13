#include "AttackSystem.h"
#include "Core/GameClock/GameClock.h"
#include "Core/GlobalStruct.h"

namespace DeltaEngine
{

    void AttackSystem::Update()
    {
        em.ForEach([&](EntityID& id, Attack& a,Image& im)
        {
            if (a.CooldownTimer > 0)
            {
                a.CooldownTimer -= env.pClock->DeltaTime();
            }

            if (a.RangeAttack)
            {
                if (a.CooldownTimer <= 0)
                {
                    RangedAttackingEntities.push_back(id);
                    a.CooldownTimer = a.MaxCooldown;
                }
                a.RangeAttack = false;
            }
            if (a.MeleeAttack)
            {
                if (a.CooldownTimer <= 0)
                {
                    MeleeAttackingEntities.push_back(id);
                    a.CooldownTimer = a.MaxCooldown;
                }
                a.MeleeAttack = false;

            }
        });

        for (auto& id : RangedAttackingEntities)
        {
            RangedAttack(id);
        }

        for (auto& id : MeleeAttackingEntities)
        {
            MeleeAttack(id);
        }

        RangedAttackingEntities.clear();
        MeleeAttackingEntities.clear();

    }
    void AttackSystem::LateUpdate()
    {

    }

    void AttackSystem::RangedAttack(EntityID& id)
    {
        if (em.GetComponent<EntityType>(id).type == EntityCategory::E_PLAYER)
        {
            //std::cout << "range attack!\n";
            Transform& t1 = em.GetComponent<Transform>(id);
            EntityID missile = em.CreateEntity<Collider, Lifespan, Transform, RigidBody, EntityType, Health>();
            em.GetComponent<Transform>(missile).position = t1.position;
            em.GetComponent<RigidBody>(missile).Mass = 5.0f;
            em.GetComponent<Transform>(missile).scale = { 0.4f,0.4f,0.0f };
            em.GetComponent<Lifespan>(missile).Timer = 0.35f;
            em.GetComponent<RigidBody>(missile).hasGravity = true;
            em.GetComponent<Collider>(missile).isTrigger = true;
            em.GetComponent<EntityType>(missile).type = EntityCategory::E_PLAYER_BULLET;
            em.GetComponent<RigidBody>(missile).FrictionCoeff = 0.0f;
            em.GetComponent<Health>(missile).CurrentHealth = 1;
            if (em.GetComponent<Image>(id).m_FlipX == false)
            {
                em.GetComponent<Transform>(missile).position.x += 0.5f;
                em.GetComponent<RigidBody>(missile).AccumulatedForce = { 7500,4000 };
            }
            else
            {
                em.GetComponent<Transform>(missile).position.x -= 0.5f;
                em.GetComponent<RigidBody>(missile).AccumulatedForce = { -7500,4000 };
            }
        }

    }

    void AttackSystem::MeleeAttack(EntityID& id)
    {
        if (em.GetComponent<EntityType>(id).type == EntityCategory::E_PLAYER)
        {
            Transform& t1 = em.GetComponent<Transform>(id);
            EntityID missile = em.CreateEntity<Collider, Lifespan, Transform, RigidBody, EntityType, Health>();
            em.GetComponent<Transform>(missile).position = t1.position;
            em.GetComponent<RigidBody>(missile).Mass = 5.0f;
            em.GetComponent<Transform>(missile).scale = { 0.7f,0.7f,0.0f };
            em.GetComponent<Lifespan>(missile).Timer = 0.1f;
            em.GetComponent<RigidBody>(missile).hasGravity = false;
            em.GetComponent<Collider>(missile).isTrigger = true;
            em.GetComponent<EntityType>(missile).type = EntityCategory::E_PLAYER_PUNCH;
            em.GetComponent<RigidBody>(missile).FrictionCoeff = 0.0f;
            em.GetComponent<Health>(missile).CurrentHealth = 1;
            if (em.GetComponent<Image>(id).m_FlipX == false)
            {
                em.GetComponent<Transform>(missile).position.x += 0.6f;
                em.GetComponent<RigidBody>(missile).AccumulatedForce = { 700,0 };
                em.GetComponent<RigidBody>(missile).Velocity = em.GetComponent<RigidBody>(id).Velocity;
            }
            else
            {
                em.GetComponent<Transform>(missile).position.x -= 0.5f;
                em.GetComponent<RigidBody>(missile).AccumulatedForce = { -500,0 };
                em.GetComponent<RigidBody>(missile).Velocity = em.GetComponent<RigidBody>(id).Velocity;
            }
        }

        if (em.GetComponent<EntityType>(id).type == EntityCategory::E_ENEMY)
        {
            Transform& t1 = em.GetComponent<Transform>(id);
            EntityID missile = em.CreateEntity<Collider, Lifespan, Transform, RigidBody, EntityType, Health>();
            em.GetComponent<Transform>(missile).position = t1.position;
            em.GetComponent<RigidBody>(missile).Mass = 5.0f;
            em.GetComponent<Transform>(missile).scale = { 0.2f,0.2f,0.0f };
            em.GetComponent<Lifespan>(missile).Timer = 0.1f;
            em.GetComponent<RigidBody>(missile).hasGravity = false;
            em.GetComponent<Collider>(missile).isTrigger = true;
            em.GetComponent<EntityType>(missile).type = EntityCategory::E_LANCER_CHARGE;
            em.GetComponent<RigidBody>(missile).FrictionCoeff = 0.0f;
            em.GetComponent<Health>(missile).CurrentHealth = 1;
            if (em.GetComponent<Image>(id).m_FlipX == false)
            {
                em.GetComponent<Transform>(missile).position.x += 0.5f;
                em.GetComponent<RigidBody>(missile).AccumulatedForce = { 500,0 };
                em.GetComponent<RigidBody>(missile).Velocity = em.GetComponent<RigidBody>(id).Velocity;
            }
            else
            {
                em.GetComponent<Transform>(missile).position.x -= 0.5f;
                em.GetComponent<RigidBody>(missile).AccumulatedForce = { -500,0 };
                em.GetComponent<RigidBody>(missile).Velocity = em.GetComponent<RigidBody>(id).Velocity;
            }
        }

    }

}
