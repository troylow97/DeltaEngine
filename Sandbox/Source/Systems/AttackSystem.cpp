#include "AttackSystem.h"
#include "Core/GameClock/GameClock.h"
#include "Core/GlobalStruct.h"
#include "Core/Debugging/Profiler/Profiler.h"
#include "AI/AITools.h"
#include "../UnitManager.h"


namespace DeltaEngine
{
    void AttackSystem::Initialize()
    {
        //PlayerAttackCombo pac;
        //pac.MaxComboNumber = 3;
        //pac.ComboMaxDuration = 1.5f;

        JsonFile file;
        //rttr::variant v{ pac };
        //auto& seq = v.create_sequential_view();
        //file.StartWriter("Player/player_attack_combo.json").StartObject().WriteObject(v).EndObject().EndWriter();

        file.StartReader("Player/player_attack_combo.json").LoadObject(_pac).EndReader();
    }

    void AttackSystem::Update()
    {
        em.ForEach([&](EntityID& id, Attack& a, Image& im)
            {
                if (a.CooldownTimer > 0)
                {
                    a.CooldownTimer -= env.pClock->DeltaTime();
                }

                Dash();

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
        Profiler::Instance().Record("Attack System");
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
            EntityID missile = em.CreateEntity<Collider, Lifespan, RigidBody, Health>();
            em.GetComponent<Transform>(missile).position = t1.position;
            em.GetComponent<RigidBody>(missile).Mass = 5.0f;
            em.GetComponent<Transform>(missile).scale = { 0.4f, 0.4f, 0.0f };
            em.GetComponent<Lifespan>(missile).Timer = 0.35f;
            em.GetComponent<RigidBody>(missile).hasGravity = true;
            em.GetComponent<Collider>(missile).isTrigger = true;
            em.GetComponent<Collider>(missile).CollisionLayerCheck = 7;
            em.GetComponent<Collider>(missile).CollisionLayerID = 8;
            em.GetComponent<EntityType>(missile).type = EntityCategory::E_PLAYER_BULLET;
            em.GetComponent<RigidBody>(missile).FrictionCoeff = 0.0f;
            em.GetComponent<Health>(missile).CurrentHealth = 1;
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

            EntityID missile2 = em.CreateEntity<Collider, Lifespan, RigidBody, Health>();
            em.GetComponent<Transform>(missile2).position = t1.position;
            em.GetComponent<RigidBody>(missile2).Mass = 5.0f;
            em.GetComponent<Transform>(missile2).scale = { 1.5f, 1.5f, 0.0f };
            em.GetComponent<Lifespan>(missile2).Timer = 0.35f;
            em.GetComponent<RigidBody>(missile2).hasGravity = true;
            em.GetComponent<Collider>(missile2).isTrigger = true;
            em.GetComponent<Collider>(missile).CollisionLayerCheck = 7;
            em.GetComponent<Collider>(missile).CollisionLayerID = 8;
            //em.GetComponent<Collider>(missile2).size = {2.0f,2.0f};
            em.GetComponent<EntityType>(missile2).type = EntityCategory::E_PLAYER_BULLET_DETECTION;
            em.GetComponent<RigidBody>(missile2).FrictionCoeff = 0.0f;
            em.GetComponent<Health>(missile2).CurrentHealth = 1;
            if (em.GetComponent<Image>(id).m_FlipX == false)
            {
                em.GetComponent<Transform>(missile2).position.x += 0.5f;
                em.GetComponent<RigidBody>(missile2).AccumulatedForce = { 8000, 3500 };
            }
            else
            {
                em.GetComponent<Transform>(missile2).position.x -= 0.5f;
                em.GetComponent<RigidBody>(missile2).AccumulatedForce = { -8000, 3500 };
            }

        }
        else if (em.GetComponent<EntityType>(id).type == EntityCategory::E_ENEMY)
        {
            Transform& t1 = em.GetComponent<Transform>(id);
            RigidBody& r1 = em.GetComponent<RigidBody>(id);
            r1.Direction = Vector2::zero();
            EntityID missile = em.CreateEntity<Collider, Lifespan, Transform, RigidBody, EntityType, Health>();
            em.GetComponent<Transform>(missile).position = t1.position;
            em.GetComponent<RigidBody>(missile).Mass = 5.0f;
            em.GetComponent<Transform>(missile).scale = { 0.4f, 0.4f, 0.0f };
            em.GetComponent<Lifespan>(missile).Timer = 0.35f;
            em.GetComponent<RigidBody>(missile).hasGravity = true;
            em.GetComponent<Collider>(missile).isTrigger = true;
            em.GetComponent<Collider>(missile).CollisionLayerCheck = 7;
            em.GetComponent<Collider>(missile).CollisionLayerID = 8;
            em.GetComponent<EntityType>(missile).type = EntityCategory::E_ENEMY_BULLET; //This bullet wont deal any damage now, to fix
            em.GetComponent<RigidBody>(missile).FrictionCoeff = 0.0f;
            em.GetComponent<Health>(missile).CurrentHealth = 1;
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
        if (em.GetComponent<EntityType>(id).type == EntityCategory::E_PLAYER)
        {
            Transform& t1 = em.GetComponent<Transform>(id);
            EntityID missile = em.CreateEntity<Collider, Lifespan, Transform, RigidBody, EntityType, Health>();
            em.GetComponent<Transform>(missile).position = t1.position;
            em.GetComponent<RigidBody>(missile).Mass = 5.0f;
            em.GetComponent<Transform>(missile).scale = { 0.7f, 0.7f, 0.0f };
            em.GetComponent<Lifespan>(missile).Timer = 0.1f;
            em.GetComponent<RigidBody>(missile).hasGravity = false;
            em.GetComponent<Collider>(missile).isTrigger = true;
            em.GetComponent<Collider>(missile).CollisionLayerCheck = 7;
            em.GetComponent<Collider>(missile).CollisionLayerID = 8;
            em.GetComponent<EntityType>(missile).type = EntityCategory::E_PLAYER_PUNCH;
            em.GetComponent<RigidBody>(missile).FrictionCoeff = 0.0f;
            em.GetComponent<Health>(missile).CurrentHealth = 1;
            if (em.GetComponent<Image>(id).m_FlipX == false)
            {
                em.GetComponent<Transform>(missile).position.x += 0.6f;
                em.GetComponent<RigidBody>(missile).AccumulatedForce = { 700, 0 };
                em.GetComponent<RigidBody>(missile).Velocity = em.GetComponent<RigidBody>(id).Velocity;
            }
            else
            {
                em.GetComponent<Transform>(missile).position.x -= 0.5f;
                em.GetComponent<RigidBody>(missile).AccumulatedForce = { -500, 0 };
                em.GetComponent<RigidBody>(missile).Velocity = em.GetComponent<RigidBody>(id).Velocity;
            }
        }

        if (em.GetComponent<EntityType>(id).type == EntityCategory::E_ENEMY)
        {
            Transform& t1 = em.GetComponent<Transform>(id);
            EntityID missile = em.CreateEntity<Collider, Lifespan, Transform, RigidBody, EntityType, Health>();
            em.GetComponent<Transform>(missile).position = t1.position;
            em.GetComponent<RigidBody>(missile).Mass = 5.0f;
            em.GetComponent<Transform>(missile).scale = { 0.2f, 0.2f, 0.0f };
            em.GetComponent<Lifespan>(missile).Timer = 0.1f;
            em.GetComponent<RigidBody>(missile).hasGravity = false;
            em.GetComponent<Collider>(missile).isTrigger = true;
            em.GetComponent<Collider>(missile).CollisionLayerCheck = 7;
            em.GetComponent<Collider>(missile).CollisionLayerID = 8;
            em.GetComponent<EntityType>(missile).type = EntityCategory::E_LANCER_CHARGE; //CHANGE TO FIDDLER
            em.GetComponent<RigidBody>(missile).FrictionCoeff = 0.0f;
            em.GetComponent<Health>(missile).CurrentHealth = 1;
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

    void AttackSystem::Dash()
    {
        em.ForEach([&](EntityID& id1, Transform& t1, EntityType et1)
            {
                if (et1.type == EntityCategory::E_PLAYER_DASH)
                {
                    if (env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(GetUnitManager().player).isCollidingOnFloor)
                        t1.position = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(GetUnitManager().player).position;
                    else
                        env.pECS->GetWorld().GetEntityManager().DestroyEntity(id1);
                    return;
                }
            });
    }

    void AttackSystem::AttackCombo()
    {
        /*
          f32 GameClock::RealDeltaTime() const
            return m_dt;

          f32 GameClock::FixedDeltaTime() const
            return m_fixed_dt * m_timescale;

          f32 GameClock::ElapsedTime() const
            return m_g_elapsed;
        */
    }
} //Namespace DeltaEngine
