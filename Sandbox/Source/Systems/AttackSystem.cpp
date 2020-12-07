#include "AttackSystem.h"
#include "UnitManager.h"
#include "Core/GameClock/EngineClock.h"
#include "Core/GlobalStruct.h"
#include "Core/Debugging/Profiler/Profiler.h"
#include "Audio/AudioEngine.h"
#include  "Core/Utils/Random.h"
namespace DeltaEngine
{
    void AttackSystem::Update()
    {
        auto& p = env.pECS->GetWorld().GetEntityManager().GetComponent<Player>(UnitManager::GetPlayerID());
    	
        if (p.IsDashing)
        {
            p.StartDashingTimer = true;
        }
    	if(p.StartDashingTimer)
    	{
            p.DashingTimerCooldown -= env.pClock->FixedDeltaTime();
            p.AllowDashing = false;
    	}
        if (p.DashingTimerCooldown <= 0.0f)
        {
            p.StartDashingTimer = false;
            p.DashingTimerCooldown = p.DashingTimerDuration;
            p.AllowDashing = true;
        }
        Dash();

        em.ForEach([&](EntityID& id, Attack& a, Image& im)
        {
            if (a.CooldownTimer > 0)
            {
                a.CooldownTimer -= env.pClock->FixedDeltaTime();
            }

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
            EntityID missile = CreateProjectile(id, Vector2{ 0.4f,0.4f }, true, 0.15f, EntityCategory::E_PLAYER_BULLET);
            EntityID missile2 = CreateProjectile(id, Vector2{ 1.7f,1.7f }, true, 0.35f, EntityCategory::E_PLAYER_BULLET_DETECTION);
            em.AddComponent<Renderer2D>(missile);
            em.AddComponent<Image>(missile);
            em.GetComponent<Renderer2D>(missile).m_SortingLayer = 4;
            em.GetComponent<Image>(missile).m_Size = { 1.0f,1.0f };
            em.GetComponent<Image>(missile).m_Sprite.m_Key = "Textures/SERP_HEAD_AIM";
            em.GetComponent<Image>(missile).m_Sprite.m_Index = 0;
            static size_t c_id{ u64_max };
            if (AudioEngine::IsChannelPlaying(c_id))
                AudioEngine::StopChannel(c_id);
            c_id  = AudioEngine::Play("Audio/jump.wav");

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
            EntityID missile = CreateProjectile(id, Vector2{ 0.4f,0.4f }, true, 0.35f, EntityCategory::E_ENEMY_BULLET);
            em.AddComponent<Renderer2D>(missile);
            em.AddComponent<Image>(missile);
            em.GetComponent<Renderer2D>(missile).m_SortingLayer = 4;
            em.GetComponent<Image>(missile).m_Size = { 1.0f,1.0f };
            em.GetComponent<Image>(missile).m_Sprite.m_Key = "Textures/SERP_HEAD_AIM";
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
            EntityID missile = CreateProjectile(id, Vector2{ 0.7f,0.5f }, false, 0.1f, EntityCategory::E_PLAYER_PUNCH);
            static size_t c_id{ u64_max };
            if (AudioEngine::IsChannelPlaying(c_id))
                AudioEngine::StopChannel(c_id);
            c_id = AudioEngine::Play("Audio/jump.wav");
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
                unsigned rand_sound = Random::RandomIntRange(0, 3);
        		switch(rand_sound)
        		{
                case 0:
                   AudioEngine::Play("Audio/Lancer/LancerCharge1.ogg");
                    break;
                case 1:
                   AudioEngine::Play("Audio/Lancer/LancerCharge2.ogg");
                    break;
                case 2:
                    AudioEngine::Play("Audio/Lancer/LancerCharge3.ogg");
                    break;
        		}

                EntityID missile = CreateProjectile(id, Vector2{ 0.2f,0.2f }, false, 0.2f, EntityCategory::E_ENEMY_LANCER_PUNCH);
                const Vector2 player_pos = em.GetComponent<Transform>(UnitManager::GetPlayerID()).position;
                const Vector2 monster_pos = em.GetComponent<Transform>(id).position;
                Vector2 kb = (player_pos - em.GetComponent<Transform>(id).position);

                em.GetComponent<Transform>(missile).position = monster_pos + kb.Normalize() * 0.5f;
                em.GetComponent<RigidBody>(missile).AccumulatedForce += kb.Normalize() * 400.0f;
                em.GetComponent<RigidBody>(missile).Velocity = em.GetComponent<RigidBody>(id).Velocity;

        		//Apply knockback to lancer
                em.GetComponent<RigidBody>(id).AccumulatedForce += -kb.Normalize() * 4000.0f;
        		
        	}
            else
            {
                AudioEngine::Play("Audio/Fiddler/FiddlerAttack.ogg");
                EntityID missile = CreateProjectile(id, Vector2{ 0.3f,0.3f }, false, 0.1f, EntityCategory::E_ENEMY_FIDDLER_PUNCH);
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
                if (env.pECS->GetWorld().GetEntityManager().GetComponent<Player>(UnitManager::GetPlayerID()).IsDashing)
                    t1.position = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(UnitManager::GetPlayerID()).position;
                else
                    env.pECS->GetWorld().GetEntityManager().DestroyEntity(id1);
            }
        });
    }

    EntityID AttackSystem::CreateProjectile(EntityID id,Vector2 scale,bool gravity,float Lifetime,EntityCategory type)
    {
        Transform& t1 = em.GetComponent<Transform>(id);
        EntityID missile = em.CreateEntity<Collider, Lifespan, RigidBody, Health>();
        em.GetComponent<Transform>(missile).position = t1.position;
        em.GetComponent<RigidBody>(missile).Mass = 5.0f;
        em.GetComponent<Collider>(missile).size = scale;
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
