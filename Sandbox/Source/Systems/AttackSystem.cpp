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
#include "Core/Utils/Random.h"
#include "MouseCalculation.h"

namespace DeltaEngine
{
  void AttackSystem::Update()
  {
  	if(em.IsEntityValid(UnitManager::GetPlayerID()) && em.HasComponent<Player>(UnitManager::GetPlayerID()))
  	{
  	  //DASH
      auto& p = env.pECS->GetWorld().GetEntityManager().GetComponent<Player>(UnitManager::GetPlayerID());
      auto& s = env.pECS->GetWorld().GetEntityManager().GetComponent<State>(UnitManager::GetPlayerID());
      auto& h = env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(UnitManager::GetPlayerID());
      if (p.IsDashing || p.IsDodging)
      {
        em.GetComponent<State>(UnitManager::GetPlayerID()).SetBool("LancerAttack", true);
        p.StartDashingTimer = true;
      }
      if (p.StartDashingTimer)
      {
        p.DashingTimerCooldown -= env.pClock->FixedDeltaTime();
        p.AllowDashing = false;
        h.isInvulnerable = true;
      }
      if (p.DashingTimerCooldown <= 0.0f)
      {
        p.StartDashingTimer = false;
        p.DashingTimerCooldown = p.DashingTimerDuration;
        h.isInvulnerable = false;
        p.AllowDashing = true;
        s.SetBool("LancerAttack", false);
      }
      Dash();
      
  	  //SMG
      auto& a = env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(UnitManager::GetPlayerID());
      
      if (a.SMGAttack && a.SMGFireRate <= 0.0f)
      {
          // em.GetComponent<State>(UnitManager::GetPlayerID()).SetBool("LancerAttack", true); // set animation
          //p.StartDashingTimer = true;
          EntityID player = UnitManager::GetPlayerID();
          SMGAttack(player);
      }
      em.ForEach([&](EntityID& id, EntityType& et, Transform& t, Image& im, Animator& anim, Renderer2D& r2d)
        {
          if (et.type == EntityCategory::E_PLAYER_BODYPART_ROTATABLE || et.type == EntityCategory::E_PLAYER_BODYPART)
            r2d.m_Active = a.SMGAttack;
        });
      if (a.SMGAttack)
      {
        a.StartSMGCooldownTimer = true;
        em.ForEach([&](EntityID& id, EntityType& et, Transform& t, Image& im, Animator& anim, Renderer2D& r2d)
        {
          if (et.type == EntityCategory::E_PLAYER_BODYPART_ROTATABLE || et.type == EntityCategory::E_PLAYER_BODYPART)
          {
            auto& player_pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(UnitManager::GetPlayerID());
            auto& player_bodypart_pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id);
      
            player_bodypart_pos.position.x = player_pos.position.x;
            player_bodypart_pos.position.y = player_pos.position.y;
      
            if (et.type == EntityCategory::E_PLAYER_BODYPART_ROTATABLE)
            {
              if (MouseCalculation::IsWithinRange(true) || MouseCalculation::IsWithinRange(false))
              {
                Vector2 direction = { MouseCalculation::CalculateDirectionVector().x, MouseCalculation::CalculateDirectionVector().y }; // CalculateDirectionVectorToShoot
                direction.Normalize();
                float angle = std::atan(direction.y / direction.x) * 180 / Math::pi;
                player_bodypart_pos.rotation = Quaternion::AngleAxis(angle * -1.0f, Vector3::forward());
              }
              else if (MouseCalculation::ShootRight() && MouseCalculation::IsWithinRange(true) == false)
              {
                Vector2 direction = { MouseCalculation::CalculateDirectionVectorToShoot().x, MouseCalculation::CalculateDirectionVectorToShoot().y };
                direction.Normalize();
                float angle = std::atan(direction.y / direction.x) * 180 / Math::pi;
                player_bodypart_pos.rotation = Quaternion::AngleAxis(angle * -1.0f, Vector3::forward());
              }
              else if (MouseCalculation::ShootLeft() && MouseCalculation::IsWithinRange(false) == false)
              {
                Vector2 direction = { -MouseCalculation::CalculateDirectionVectorToShoot().x, MouseCalculation::CalculateDirectionVectorToShoot().y };
                direction.Normalize();
                float angle = std::atan(direction.y / direction.x) * 180 / Math::pi;
                player_bodypart_pos.rotation = Quaternion::AngleAxis(angle * -1.0f, Vector3::forward());
              }
              else
                player_bodypart_pos.rotation = Quaternion::Identity();
      
              auto& player_image = env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(UnitManager::GetPlayerID());
              auto& player_bodypart_image = env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(id);
              player_bodypart_image.m_FlipX = player_image.m_FlipX * -1;
              player_bodypart_image.m_FlipY = true;
            }
            else
            {
              auto& player_image = env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(UnitManager::GetPlayerID());
              auto& player_bodypart_image = env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(id);
              player_bodypart_image.m_FlipX = player_image.m_FlipX * -1;
              player_bodypart_image.m_FlipY = false;
            }
          }
        });
      }
      if (a.StartSMGCooldownTimer)
      {
        if (a.SMGFireRate >= 0.0f)
        {
          a.SMGFireRate -= env.pClock->FixedDeltaTime();
          a.AllowSMGAttack = false;
        }
        else
        {
          a.SMGFireRate = a.SMGCooldown;
          a.AllowSMGAttack = true;
          a.StartSMGCooldownTimer = false;
        }
      }
  	}

    // melee and ranged attack ----------------------------------------------------------------------------------
    em.ForEach([&](EntityID& id,EntityType et, RigidBody& r, Attack& a, Image& im, Animator& anim, State& st)
    {
      //Reduce Cooldowns   	
      if (a.MeleeCooldownTimer > -0.2f)
        a.MeleeCooldownTimer -= env.pClock->FixedDeltaTime();

      if (a.RangeCooldownTimer > -0.2f)
        a.RangeCooldownTimer -= env.pClock->FixedDeltaTime();

      if(a.AttackDelay > 0.0f)
        a.AttackDelay -= env.pClock->FixedDeltaTime();

      //Toggle Ranged Attack
      if (a.RangeAttack && a.RangeCooldownTimer <= 0 && a.AttackDelay < 0.0f)
      {
        em.GetComponent<State>(id).SetBool("Ranged", true);
        RangedAttackingEntities.push_back(id);
        a.RangeCooldownTimer = a.RangeCooldown;
        a.RangeAttack = false;
      }
      if (a.RangeCooldownTimer <= (a.RangeCooldown - 0.5f))
          em.GetComponent<State>(id).SetBool("Ranged", false);

       //Toggle Player Melee Attack
    	if(a.MeleeAttack && a.MeleeCooldownTimer < 0.0f)
    	{
            if (et.type == EntityCategory::E_PLAYER && a.AttackDelay < 0.0f)
            {  	
                ++a.NumberOfCombos;
                if (a.NumberOfCombos == 1)
                {
                    a.StartComboCooldownTimer = true;
                    st.SetBool("Punch1", true);
                    st.SetBool("Punch2", false);
                    st.SetBool("Punch3", false);
                    AudioEngine::SetGlobalParameterByName("Punch", 1);
                    MeleeAttackingEntities.push_back(id);
                    a.MeleeCooldownTimer = a.MeleeCooldown;
                    a.MeleeAttack = false;
                }
                else if (a.NumberOfCombos == 2)
                {
                    st.SetBool("Punch2", true);
                    st.SetBool("Punch1", false);
                    st.SetBool("Punch3", false);
                    AudioEngine::SetGlobalParameterByName("Punch", 2);
                    MeleeAttackingEntities.push_back(id);
                    a.MeleeCooldownTimer = a.MeleeCooldown;
                    a.MeleeAttack = false;

                }
                else if (a.NumberOfCombos == 3)
                {                	
                    st.SetBool("Punch3", true);
                    st.SetBool("Punch1", false);
                    st.SetBool("Punch2", false);
                    AudioEngine::SetGlobalParameterByName("Punch", 3);
                    MeleeAttackingEntities.push_back(id);
                    a.MeleeCooldownTimer = a.MeleeCooldown;
                    a.MeleeAttack = false;
                    a.NumberOfCombos = 0;
                    a.AttackDelay = 0.7f;
                }
            	
            }
            else if(et.type != EntityCategory::E_PLAYER)
            {
                MeleeAttackingEntities.push_back(id);
                a.MeleeCooldownTimer = a.MeleeCooldown;
                a.MeleeAttack = false;
            }
            else if(a.AttackDelay < 0.3f)
            {
                st.SetBool("Punch3", false);
            }

            if (a.StartComboCooldownTimer)
            {
                a.ComboCooldownTimer -= env.pClock->FixedDeltaTime();
                if (a.ComboCooldownTimer < 0.0f)
                {
                    st.SetBool("Punch1", false);
                    st.SetBool("Punch2", false);
                    st.SetBool("Punch3", false);
                    a.NumberOfCombos = 0;
                    a.StartComboCooldownTimer = false;
                    a.ComboCooldownTimer = a.ComboDuration;
                }
            }
    	}


    });
  	

    for (auto& id1 : RangedAttackingEntities)
    {
      RangedAttack(id1);
    }

    for (auto& id1 : MeleeAttackingEntities)
    {
      MeleeAttack(id1);
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
      em.GetComponent<Image>(missile).m_Size = {0.6f, 0.6f};
      em.GetComponent<Image>(missile).m_Sprite.m_Key = "Textures/SERP_HEAD_AIM";
      em.GetComponent<Image>(missile).m_Sprite.m_Index = 0;
      em.GetComponent<State>(id).SetBool("Ranged", true);
      static size_t c_id{u64_max};
      AudioEngine::Play2DEvent( "event:/Player/PlayerJump" );


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
    else if (em.GetComponent<EntityType>(id).type == EntityCategory::E_ENEMY)
    {
      EntityID missile = CreateProjectile(id, Vector2{ 0.25f, 0.25f }, false, 1.0f, EntityCategory::E_ENEMY_BULLET);
      Vector2 direction_to_shoot = { CalculateAttackDirection(id).x, CalculateAttackDirection(id).y };
      Transform& enemy_pos = em.GetComponent<Transform>(id);
      Collider& enemy_collider = em.GetComponent<Collider>(id);
      em.AddComponent<Renderer2D>(missile);
      em.AddComponent<Image>(missile);
      em.GetComponent<Renderer2D>(missile).m_SortingLayer = 4;
      em.GetComponent<Image>(missile).m_Size = { 0.6f, 0.6f };
      em.GetComponent<Image>(missile).m_Sprite.m_Key = "Textures/SERP_BULLET";
      em.GetComponent<Transform>(missile).position = { enemy_pos.position.x, enemy_pos.position.y + (enemy_collider.size.y / 2 * 0.75f), enemy_pos.position.z };

      if (em.GetComponent<Image>(id).m_FlipX == false)
      {
        //em.GetComponent<Transform>(missile).position.x += 0.4f;
        em.GetComponent<Transform>(missile).position.y -= 0.2f;
        em.GetComponent<RigidBody>(missile).AccumulatedForce = { direction_to_shoot.x * 7000, direction_to_shoot.y * 7000 }; // -7000, -2500
      }
      else
      {
        //em.GetComponent<Transform>(missile).position.x -= 0.4f;
        em.GetComponent<Transform>(missile).position.y -= 0.2f;
        em.GetComponent<RigidBody>(missile).AccumulatedForce = { direction_to_shoot.x * 7000, direction_to_shoot.y * 7000 }; //  7000, -2500
      }
    }
  }

  void AttackSystem::MeleeAttack(EntityID& id)
  {
  	//If Player is attacking
    if (em.GetComponent<EntityType>(id).type == EntityCategory::E_PLAYER && env.pECS->GetWorld().GetEntityManager().
      HasComponent<Attack>(id))
    {
      EntityID missile = CreateProjectile(id, Vector2{0.5f, 0.4f}, false, 0.1f, EntityCategory::E_PLAYER_PUNCH);
      static size_t c_id{u64_max};
   	
      AudioEngine::Play2DEvent( "event:/Player/PlayerPunch" );
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
      return;
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
        //
        ////Apply knockback to lancer
        //em.GetComponent<RigidBody>(id).AccumulatedForce += -kb.Normalize() * 8000.0f;
      }
      else
      {
        static size_t c_id{u64_max};
        if (AudioEngine::IsChannelPlaying(c_id))
          AudioEngine::StopChannel(c_id);
        c_id = AudioEngine::Play("Audio/Fiddler/FiddlerAttack.ogg");
        EntityID missile = CreateProjectile(id, Vector2{0.3f, 0.3f}, false, 0.1f,
                                            EntityCategory::E_ENEMY_FIDDLER_PUNCH);
        if (em.GetComponent<Image>(id).m_FlipX == true)
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

  void AttackSystem::SMGAttack(EntityID& id)
  {
    if (em.GetComponent<EntityType>(id).type == EntityCategory::E_PLAYER)
    {
      EntityID smgbullet = CreateSMGBullet(id, Vector2{ 0.25f, 0.25f }, false, 0.5f, EntityCategory::E_PLAYER_SMG);
      em.AddComponent<Renderer2D>(smgbullet);
      em.AddComponent<Image>(smgbullet);
      em.GetComponent<Renderer2D>(smgbullet).m_SortingLayer = 4;
      em.GetComponent<Image>(smgbullet).m_Size = { 0.25f, 0.25f };
      em.GetComponent<Image>(smgbullet).m_Sprite.m_Key = "Textures/DAVE_BULLET"; 
      em.GetComponent<Image>(smgbullet).m_Sprite.m_Index = 0;

      ////// em.GetComponent<State>(id).SetBool("Ranged", true); // change when have the animation 
      //////static size_t c_id{ u64_max };
      //////if (AudioEngine::IsChannelPlaying(c_id))
      //////  AudioEngine::StopChannel(c_id);
      //////c_id = AudioEngine::Play("Audio/SWORD_GEN-HDF-22317.wav");
      
      if (MouseCalculation::ShootRight() && MouseCalculation::IsWithinRange(true) == true)
      {
        Vector2 direction_to_shoot = { MouseCalculation::CalculateDirectionVector().x, MouseCalculation::CalculateDirectionVector().y };
        em.GetComponent<RigidBody>(smgbullet).AccumulatedForce = { direction_to_shoot.x * 7000, direction_to_shoot.y * 7000 };
      }
      else if (MouseCalculation::ShootRight() && MouseCalculation::IsWithinRange(true) == false)
      {
        Vector2 direction_to_shoot = { MouseCalculation::CalculateDirectionVectorToShoot().x, MouseCalculation::CalculateDirectionVectorToShoot().y };
        em.GetComponent<RigidBody>(smgbullet).AccumulatedForce = { direction_to_shoot.x * 7000, direction_to_shoot.y * 7000 };
      }
      else if (MouseCalculation::ShootLeft() && MouseCalculation::IsWithinRange(false) == true)
      {
        Vector2 direction_to_shoot = { MouseCalculation::CalculateDirectionVector().x, MouseCalculation::CalculateDirectionVector().y };
        em.GetComponent<RigidBody>(smgbullet).AccumulatedForce = { direction_to_shoot.x * 7000, direction_to_shoot.y * 7000 };
      }
      else if (MouseCalculation::ShootLeft() && MouseCalculation::IsWithinRange(false) == false)
      {
        Vector2 direction_to_shoot = { -MouseCalculation::CalculateDirectionVectorToShoot().x, MouseCalculation::CalculateDirectionVectorToShoot().y };
        em.GetComponent<RigidBody>(smgbullet).AccumulatedForce = { direction_to_shoot.x * 7000, direction_to_shoot.y * 7000 };
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

  EntityID AttackSystem::CreateSMGBullet(EntityID id, Vector2 scale, bool gravity, float Lifetime, EntityCategory type)
  {
    Transform& t1 = em.GetComponent<Transform>(id);
    EntityID smgbullet = em.CreateEntity<Collider, Lifespan, RigidBody>();
    em.GetComponent<Transform>(smgbullet).position.x = t1.position.x;// -0.5f;
    em.GetComponent<Transform>(smgbullet).position.y = t1.position.y;
    em.GetComponent<RigidBody>(smgbullet).Mass = 5.0f;
    em.GetComponent<Transform>(smgbullet).scale = scale;
    em.GetComponent<Lifespan>(smgbullet).Timer = Lifetime;
    em.GetComponent<RigidBody>(smgbullet).hasGravity = gravity;
    em.GetComponent<Collider>(smgbullet).isTrigger = true;
    em.GetComponent<Collider>(smgbullet).CollisionLayerCheck = 7;
    em.GetComponent<Collider>(smgbullet).CollisionLayerID = 8;
    em.GetComponent<EntityType>(smgbullet).type = type;
    em.GetComponent<RigidBody>(smgbullet).FrictionCoeff = 0.0f;
    return smgbullet;
  }

  Vector2 AttackSystem::CalculateAttackDirection(EntityID& enemy)
  {
    Transform& player_pos = em.GetComponent<Transform>(UnitManager::GetPlayerID());
    Transform& enemy_pos = em.GetComponent<Transform>(enemy);
    Collider& enemy_collider = em.GetComponent<Collider>(enemy);

    Vector3 serpentipede_head = { enemy_pos.position.x, enemy_pos.position.y + (enemy_collider.size.y / 2 * 0.6f), enemy_pos.position.z };
    Vector2 direction_vector = { player_pos.position.x - serpentipede_head.x, player_pos.position.y - serpentipede_head.y };
    float magnitude = direction_vector.Magnitude();
    Vector2 normalized_direction_vector = { direction_vector.x / magnitude, direction_vector.y / magnitude };
    
    return normalized_direction_vector;
  }
} //Namespace DeltaEngine
