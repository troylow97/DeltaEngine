/**********************************************************************************
* \file   AttackSystem.cpp
* \brief  The file contains the system for gameplay attack
* \author Chin, Clara,   50% Code Contribution
* \author Low, Troy,     50% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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
    if (em.IsEntityValid(UnitManager::GetPlayerID()) && em.HasComponent<Player>(UnitManager::GetPlayerID()))
    {
      auto& p = env.pECS->GetWorld().GetEntityManager().GetComponent<Player>(UnitManager::GetPlayerID());
      auto& s = env.pECS->GetWorld().GetEntityManager().GetComponent<State>(UnitManager::GetPlayerID());
      if (p.IsDashing)
      {
        em.GetComponent<State>(UnitManager::GetPlayerID()).SetBool("LancerAttack", true);
        p.StartDashingTimer = true;
      }
      if (p.StartDashingTimer)
      {
        p.DashingTimerCooldown -= env.pClock->FixedDeltaTime();
        p.AllowDashing = false;
      }
      if (p.DashingTimerCooldown <= 0.0f)
      {
        p.StartDashingTimer = false;
        p.DashingTimerCooldown = p.DashingTimerDuration;
        p.AllowDashing = true;
        s.SetBool("LancerAttack", false);
      }
      Dash();
    }

    em.ForEach([&](EntityID& id, Attack& a, Image& im, Animator& anim, State& st)
    {
      if (a.MeleeCooldownTimer > -0.2)
        a.MeleeCooldownTimer -= env.pClock->FixedDeltaTime();
      else
        em.GetComponent<State>(id).SetBool("MeleeAttack", false);

      if (a.RangeCooldownTimer > -0.2)
        a.RangeCooldownTimer -= env.pClock->FixedDeltaTime();
      else
        em.GetComponent<State>(id).SetBool("Ranged", false);

      if (a.RangeAttack && a.RangeCooldownTimer <= 0)
      {
        em.GetComponent<State>(id).SetBool("Ranged", true);
        RangedAttackingEntities.push_back(id);
        a.RangeCooldownTimer = a.RangeCooldown;
        a.RangeAttack = false;
      }

      if (a.MeleeAttack && em.HasComponent<AI>(id))
        st.SetBool("MeleeAttack", true);
      if (a.MeleeAttack && a.MeleeCooldownTimer <= 0)
      {
        a.StartComboCooldownTimer = true;
        if (a.NumberOfCombos != a.MaxComboNumber)
        {
          a.NumberOfCombos++;
          if (a.NumberOfCombos == 1)
          {
            st.SetBool("Punch1", true);
            st.SetBool("Punch2", false);
            st.SetBool("Punch3", false);
          }
          else if (a.NumberOfCombos == 2)
          {
            st.SetBool("Punch2", true);
            st.SetBool("Punch1", false);
            st.SetBool("Punch3", false);
          }
          else if (a.NumberOfCombos == 3)
          {
            st.SetBool("Punch3", true);
            st.SetBool("Punch1", false);
            st.SetBool("Punch2", false);
            a.NumberOfCombos = 0;
          }
        }

        MeleeAttackingEntities.push_back(id);
        a.MeleeCooldownTimer = a.MeleeCooldown;
      }

      a.MeleeAttack = false;


      if (a.StartComboCooldownTimer)
      {
        a.ComboCooldownTimer -= env.pClock->FixedDeltaTime();
        if (!(a.ComboCooldownTimer > 0))
        {
          st.SetBool("Punch1", false);
          st.SetBool("Punch2", false);
          st.SetBool("Punch3", false);
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
      EntityID missile = CreateProjectile(id, Vector2{0.4f, 0.4f}, true, 0.35f, EntityCategory::E_PLAYER_BULLET);
      EntityID missile2 = CreateProjectile(id, Vector2{1.7f, 1.7f}, true, 0.35f,
                                           EntityCategory::E_PLAYER_BULLET_DETECTION);
      em.AddComponent<Renderer2D>(missile);
      em.AddComponent<Image>(missile);
      em.GetComponent<Renderer2D>(missile).m_SortingLayer = 4;
      em.GetComponent<Image>(missile).m_Size = {1.0f, 1.0f};
      em.GetComponent<Image>(missile).m_Sprite.m_Key = "Textures/SERP_HEAD_AIM";
      em.GetComponent<Image>(missile).m_Sprite.m_Index = 0;
      em.GetComponent<State>(id).SetBool("Ranged", true);
      static size_t c_id{u64_max};
      if (AudioEngine::IsChannelPlaying(c_id))
        AudioEngine::StopChannel(c_id);
      c_id = AudioEngine::Play("Audio/jump.wav");

      if (em.GetComponent<Image>(id).m_FlipX == false)
      {
        em.GetComponent<Transform>(missile).position.x += 0.4f;
        em.GetComponent<RigidBody>(missile).AccumulatedForce = {8000, 3500};
        em.GetComponent<Transform>(missile2).position.x += 0.5f;
        em.GetComponent<RigidBody>(missile2).AccumulatedForce = {8000, 3500};
      }
      else
      {
        em.GetComponent<Transform>(missile).position.x -= 0.4f;
        em.GetComponent<RigidBody>(missile).AccumulatedForce = {-8000, 3500};
        em.GetComponent<Transform>(missile2).position.x -= 0.5f;
        em.GetComponent<RigidBody>(missile2).AccumulatedForce = {-8000, 3500};
      }
    }
    else if (em.GetComponent<EntityType>(id).type == EntityCategory::E_ENEMY)
    {
      EntityID missile = CreateProjectile(id, Vector2{0.4f, 0.4f}, true, 0.35f, EntityCategory::E_ENEMY_BULLET);
      em.AddComponent<Renderer2D>(missile);
      em.AddComponent<Image>(missile);
      em.GetComponent<Renderer2D>(missile).m_SortingLayer = 4;
      em.GetComponent<Image>(missile).m_Size = {1.0f, 1.0f};
      em.GetComponent<Image>(missile).m_Sprite.m_Key = "Textures/SERP_HEAD_AIM";

      if (em.GetComponent<Image>(id).m_FlipX == false)
      {
        em.GetComponent<Transform>(missile).position.x += 0.4f;
        em.GetComponent<RigidBody>(missile).AccumulatedForce = {8000, 3500};
      }
      else
      {
        em.GetComponent<Transform>(missile).position.x -= 0.4f;
        em.GetComponent<RigidBody>(missile).AccumulatedForce = {-8000, 3500};
      }
    }
  }

  void AttackSystem::MeleeAttack(EntityID& id)
  {
    if (em.GetComponent<EntityType>(id).type == EntityCategory::E_PLAYER && env.pECS->GetWorld().GetEntityManager().
      HasComponent<Attack>(id))
    {
      EntityID missile = CreateProjectile(id, Vector2{0.5f, 0.4f}, false, 0.1f, EntityCategory::E_PLAYER_PUNCH);
      static size_t c_id{u64_max};
      if (AudioEngine::IsChannelPlaying(c_id))
        AudioEngine::StopChannel(c_id);
      c_id = AudioEngine::Play("Audio/jump.wav");
      if (em.GetComponent<Image>(id).m_FlipX == false)
      {
        em.GetComponent<Transform>(missile).position.x += 0.6f;
        em.GetComponent<RigidBody>(missile).AccumulatedForce = {400, 0};
        em.GetComponent<RigidBody>(missile).Velocity = em.GetComponent<RigidBody>(id).Velocity;
      }
      else
      {
        em.GetComponent<Transform>(missile).position.x -= 0.6f;
        em.GetComponent<RigidBody>(missile).AccumulatedForce = {-400, 0};
        em.GetComponent<RigidBody>(missile).Velocity = em.GetComponent<RigidBody>(id).Velocity;
      }
    }

    if (em.GetComponent<EntityType>(id).type == EntityCategory::E_ENEMY)
    {
      if (em.GetComponent<RigidBody>(id).hasGravity == false)
      {
        unsigned rand_sound = Random::RandomIntRange(0, 3);
        switch (rand_sound)
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

        EntityID missile = CreateProjectile(id, Vector2{0.2f, 0.2f}, false, 0.2f, EntityCategory::E_ENEMY_LANCER_PUNCH);
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
        static size_t c_id{u64_max};
        if (AudioEngine::IsChannelPlaying(c_id))
          AudioEngine::StopChannel(c_id);
        c_id = AudioEngine::Play("Audio/Fiddler/FiddlerAttack.ogg");
        EntityID missile = CreateProjectile(id, Vector2{0.3f, 0.3f}, false, 0.1f,
                                            EntityCategory::E_ENEMY_FIDDLER_PUNCH);
        if (em.GetComponent<Image>(id).m_FlipX == false)
        {
          em.GetComponent<Transform>(missile).position.x += 0.5f;
          em.GetComponent<RigidBody>(missile).AccumulatedForce = {500, 0};
          em.GetComponent<RigidBody>(missile).Velocity = em.GetComponent<RigidBody>(id).Velocity;
        }
        else
        {
          em.GetComponent<Transform>(missile).position.x -= 0.5f;
          em.GetComponent<RigidBody>(missile).AccumulatedForce = {-500, 0};
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
        if (em.GetComponent<Player>(UnitManager::GetPlayerID()).IsDashing)
          t1.position = em.GetComponent<Transform>(UnitManager::GetPlayerID()).position;
        else
        {
          em.DestroyEntity(id1);
          em.GetComponent<State>(UnitManager::GetPlayerID()).SetBool("LancerAttack", false);
        }
      }
    });
  }

  EntityID AttackSystem::CreateProjectile(EntityID id, Vector2 scale, bool gravity, float Lifetime, EntityCategory type)
  {
    Transform& t1 = em.GetComponent<Transform>(id);
    EntityID missile = em.CreateEntity<Collider, Lifespan, RigidBody>();
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
    return missile;
  }
} //Namespace DeltaEngine
