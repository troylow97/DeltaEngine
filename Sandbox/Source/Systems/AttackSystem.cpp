#include "AttackSystem.h"
#include "UnitManager.h"
#include "Core/GameClock/GameClock.h"
#include "Core/GlobalStruct.h"
#include "Core/Debugging/Profiler/Profiler.h"

namespace DeltaEngine
{
    void AttackSystem::Update()
    {
        em.ForEach([&](EntityID& id, Attack& a, Image& im)
        {
            if (a.CooldownTimer > 0)
            {
                a.CooldownTimer -= env.pClock->DeltaTime();
            }
            if (env.pECS->GetWorld().GetEntityManager().HasComponent<Player>(id))
            {
                auto& p = env.pECS->GetWorld().GetEntityManager().GetComponent<Player>(id);

                if (p.isDashing)
                {
                    p.startDashingTimer = true;
                }
                if (p.startDashingTimer)
                {
                    p.dashingTimerCooldown -= env.pClock->DeltaTime();
                }
                if (p.dashingTimerCooldown <= 0.0f)
                {
                    p.startDashingTimer = false;
                    p.dashingTimerCooldown = p.dashingTimerDuration;
                }
                if (p.dashingTimerCooldown == p.dashingTimerDuration)
                {
                    p.allowDashing = true;
                }
            }

            Dash();

            if (a.RangeAttack)
            {
                if (a.CooldownTimer <= 0)
                {
                    RangedAttackingEntities.push_back(id);
                    a.CooldownTimer = a.AttackCooldown;
                }
                a.RangeAttack = false;
            }
        	
            if (a.MeleeAttack)
            {
                a.StartComboCooldownTimer = true;
                if (a.NumberOfCombos != a.MaxComboNumber)
                {
                    a.NumberOfCombos++;
                }
                if (a.CooldownTimer <= 0)
                {
                    MeleeAttackingEntities.push_back(id);
                    a.CooldownTimer = a.AttackCooldown;
                }
                a.MeleeAttack = false;
            }
            if (a.StartComboCooldownTimer)
            {
                if (a.ComboCooldownTimer > 0)
                {
                    a.ComboCooldownTimer -= env.pClock->DeltaTime();
                }
                else
                {
                    a.NumberOfCombos = 0;
                    a.StartComboCooldownTimer = false;
                    a.ComboCooldownTimer = a.ComboDuration;
                }
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
        Profiler::Instance().Record("Attack System");
    }

    void AttackSystem::LateUpdate()
    {
    }

    void AttackSystem::RangedAttack(EntityID& id)
    {
        if (em.GetComponent<EntityType>(id).type == EntityCategory::E_PLAYER)
        {
            EntityID missile = CreateProjectile(id, Vector2{ 0.4,0.4 }, true, 0.35f, EntityCategory::E_PLAYER_BULLET);
            EntityID missile2 = CreateProjectile(id, Vector2{ 1.7,1.7 }, true, 0.35f, EntityCategory::E_PLAYER_BULLET_DETECTION);
            if (em.GetComponent<Image>(id).m_FlipX == false)
            {
                em.GetComponent<Transform>(missile).position.x += 0.4f;
                em.GetComponent<RigidBody>(missile).AccumulatedForce = { 8000, 3500 };
                em.GetComponent<Transform>(missile2).position.x += 0.5f;
                em.GetComponent<RigidBody>(missile2).AccumulatedForce = { 8000, 3500 };
            }
            else
            {
                em.GetComponent<Transform>(missile).position.x -= 0.4f;
                em.GetComponent<RigidBody>(missile).AccumulatedForce = { -8000, 3500 };
                em.GetComponent<Transform>(missile2).position.x -= 0.5f;
                em.GetComponent<RigidBody>(missile2).AccumulatedForce = { -8000, 3500 };
            }

        }
        else if (em.GetComponent<EntityType>(id).type == EntityCategory::E_ENEMY)
        {
            EntityID missile = CreateProjectile(id, Vector2{ 0.4,0.4 }, true, 0.35f, EntityCategory::E_ENEMY_BULLET);
            if (em.GetComponent<Image>(id).m_FlipX == false)
            {
                em.GetComponent<Transform>(missile).position.x += 0.4f;
                em.GetComponent<RigidBody>(missile).AccumulatedForce = { 8000, 3500 };
            }
            else
            {
                em.GetComponent<Transform>(missile).position.x -= 0.4f;
                em.GetComponent<RigidBody>(missile).AccumulatedForce = { -8000, 3500 };
            }
        }
    }

    void AttackSystem::MeleeAttack(EntityID& id)
    {
        if (em.GetComponent<EntityType>(id).type == EntityCategory::E_PLAYER && env.pECS->GetWorld().GetEntityManager().HasComponent<Attack>(id))
        {
            EntityID missile = CreateProjectile(id, Vector2{ 0.7,0.5 }, false, 0.1f, EntityCategory::E_PLAYER_PUNCH);
            if (em.GetComponent<Image>(id).m_FlipX == false)
            {
                em.GetComponent<Transform>(missile).position.x += 0.6f;
                em.GetComponent<RigidBody>(missile).AccumulatedForce = { 1450, 0 };
                em.GetComponent<RigidBody>(missile).Velocity = em.GetComponent<RigidBody>(id).Velocity;
            }
            else
            {
                em.GetComponent<Transform>(missile).position.x -= 0.6f;
                em.GetComponent<RigidBody>(missile).AccumulatedForce = { -1450, 0 };
                em.GetComponent<RigidBody>(missile).Velocity = em.GetComponent<RigidBody>(id).Velocity;
            }
        }

        if (em.GetComponent<EntityType>(id).type == EntityCategory::E_ENEMY)
        {
        	if(em.GetComponent<RigidBody>(id).hasGravity == false)
        	{
                EntityID missile = CreateProjectile(id, Vector2{ 0.3,0.3 }, false, 0.1f, EntityCategory::E_ENEMY_LANCER_PUNCH);
                const Vector2 player_pos = em.GetComponent<Transform>(UnitManager::GetPlayerID()).position;
                const Vector2 monster_pos = em.GetComponent<Transform>(id).position;
                Vector2 kb = (player_pos - em.GetComponent<Transform>(id).position);

                em.GetComponent<Transform>(missile).position = monster_pos + kb.Normalize() * 0.5f;
                em.GetComponent<RigidBody>(missile).AccumulatedForce += kb.Normalize() * 500.0f;
                em.GetComponent<RigidBody>(missile).Velocity = em.GetComponent<RigidBody>(id).Velocity;

        		//Apply knockback to lancer
                em.GetComponent<RigidBody>(id).AccumulatedForce += -kb.Normalize() * 4000.0f;
        		
        	}
            else
            {
                EntityID missile = CreateProjectile(id, Vector2{ 0.3,0.3 }, false, 0.1f, EntityCategory::E_ENEMY_FIDDLER_PUNCH);
                if (em.GetComponent<Image>(id).m_FlipX == false)
                {
                    em.GetComponent<Transform>(missile).position.x += 0.5f;
                    em.GetComponent<RigidBody>(missile).AccumulatedForce = { 500, 0 };
                    em.GetComponent<RigidBody>(missile).Velocity = em.GetComponent<RigidBody>(id).Velocity;
                }
                else
                {
                    em.GetComponent<Transform>(missile).position.x -= 0.5f;
                    em.GetComponent<RigidBody>(missile).AccumulatedForce = { -500, 0 };
                    em.GetComponent<RigidBody>(missile).Velocity = em.GetComponent<RigidBody>(id).Velocity;
                }
            }

        }
    }

    void AttackSystem::Dash()
    {
        em.ForEach([&](EntityID& id1, Transform& t1, EntityType et1)
        {
            if (et1.type == EntityCategory::E_PLAYER_DASH)
            {
                if (env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(UnitManager::GetPlayerID()).isCollidingOnFloor)
                    t1.position = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(UnitManager::GetPlayerID()).position;
                else
                    env.pECS->GetWorld().GetEntityManager().DestroyEntity(id1);
            }
        });
    }

    EntityID AttackSystem::CreateProjectile(EntityID id,Vector2 scale,bool gravity,float Lifetime,EntityCategory type)
    {
        Transform& t1 = em.GetComponent<Transform>(id);
        EntityID missile = em.CreateEntity<Collider, Lifespan, Transform, RigidBody, EntityType, Health>();
        em.GetComponent<Transform>(missile).position = t1.position;
        em.GetComponent<RigidBody>(missile).Mass = 5.0f;
        em.GetComponent<Transform>(missile).scale = scale;
        em.GetComponent<Lifespan>(missile).Timer = Lifetime;
        em.GetComponent<RigidBody>(missile).hasGravity = gravity;
        em.GetComponent<Collider>(missile).isTrigger = true;
        em.GetComponent<Collider>(missile).CollisionLayerCheck = 7;
        em.GetComponent<Collider>(missile).CollisionLayerID = 8;
        em.GetComponent<EntityType>(missile).type = type;
        em.GetComponent<RigidBody>(missile).FrictionCoeff = 0.0f;
        em.GetComponent<Health>(missile).CurrentHealth = 1;
        return missile;
    }
	
} //Namespace DeltaEngine
