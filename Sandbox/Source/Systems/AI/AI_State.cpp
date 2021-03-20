/**********************************************************************************
* \file   AI_State.cpp
* \brief  The file contains the code for AI States
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "AI_State.h"
#include "AI_Transition.h"
#include "../UnitManager.h"
#include "Core/Utils/Random.h"
#include "Core/GlobalStruct.h"
#include "Core/GameClock/EngineClock.h"
#include "Audio/AudioEngine.h"

namespace DeltaEngine
{
    bool AIState::CheckEdges(EntityID& monster)
    {
        for (auto& it : TransitionEdges)
        {
            if (it.second->TestEdge(monster))
                return true;
        }

        return false;
    }

    AIState::~AIState()
    {
        for (auto& it : TransitionEdges)
        {
            delete it.second;
        }
    }

#pragma region Lancer
    /////////////////////////////////////////////////////////////////////////////////////

    LancerSpawn::LancerSpawn(Vector2& charge_range)
    {
        TransitionEdges["detect_enemy_lancer"] = new DetectEnemyLancer(charge_range);
    }

    void LancerSpawn::onEnter(EntityID& id)
    {
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id).SetBool("IsAlerted", true);

    }

    void LancerSpawn::onExit(EntityID& id)
    {
    }

    void LancerSpawn::Update(EntityID& monster)
    {
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster).SetBool("IsDead", false);
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster).SetBool("IsAlerted", true);
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster).SetBool("MeleeAttack", false);
        CheckEdges(monster);
        auto& ref = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(monster).position;
        Vector2 player_pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(UnitManager::GetPlayerID()).position;

        if (ref.y > player_pos.y)
        {
            AITools::FlyTowardsPoint(monster, player_pos);
        }
    }

    //////////////////////////////////////////////////////////////////////////////////	

    IdleLancer::IdleLancer(Vector2& charge_range)
    {
        TransitionEdges["detect_enemy_lancer"] = new DetectEnemyLancer(charge_range);
    }

    void IdleLancer::onEnter(EntityID& id)
    {
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id).SetBool("IsAttacked", false);
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id).SetBool("IsAlerted", true);
        //std::cout << "IdleLancer onEnter" << std::endl;
    }

    void IdleLancer::onExit(EntityID& id)
    {
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id).SetBool("IsIdle", false);
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id).SetBool("IsAlerted", false);

    }

    void IdleLancer::Update(EntityID& monster)
    {
        CheckEdges(monster);
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster).SetBool("IsAttacked", false);
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster).SetBool("IsDead", false);
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster).SetBool("IsAlerted", true);
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster).SetBool("MeleeAttack", false);
        //env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(monster).Movespeed = 32.0f;
        //std::cout << "IdleLancer Update" << std::endl;
        //auto& ref = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(monster).position;

        //if (ref.y < 2.0)
        //    AITools::MoveTowardsPoint(monster, Vector2{ ref.x,Random::RandomFloatRange(2.1,2.5) });
    }

    //----------------------------------------------------------------------
    ChaseEnemyLancer::ChaseEnemyLancer(Vector2 v) :
	HasEntered{false},
	ChargeDetectRange{v}
    {
        TransitionEdges["hit_enemy_lancer"] = new DamagedEnemyLancer();
    }

    void ChaseEnemyLancer::onEnter(EntityID& id)
    {
        auto& state = env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id);
        //auto& rb = env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(id);
        state.SetBool("IsAttacked", false);
        state.SetBool("IsAlerted", false);
        state.SetBool("BeginCharging", true);    	
    	if(AITools::isFacingLeft(id))
    	{
            env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(id).AccumulatedForce.x -= 3000;
    	}
        else
        {
            env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(id).AccumulatedForce.x += 3000;
        }
        //std::cout << "ChaseEnemyLancer onEnter" << std::endl;
        TransitionTimer = 0.4f;
        HasEntered = true;
    }

    void ChaseEnemyLancer::onExit(EntityID& id)
    {
        auto& state = env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id);
        state.SetBool("BeginCharging", false);
    }

    void ChaseEnemyLancer::Update(EntityID& monster)
    {
        auto& state = env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster);
        //auto& anim = env.pECS->GetWorld().GetEntityManager().GetComponent<Animator>(monster);
        //auto& rb = env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(monster);
        auto& ai = env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(monster);
        auto player = UnitManager::GetPlayerID();
    	if(!HasEntered)
    	{
            if(AITools::Distance_X_BetweenTwoEntities(monster, player) > ChargeDetectRange.x)
            {
                ai.transition = "idle_lancer";
                return;
            }
    		
            onEnter(monster);
            HasEntered = true;
    	}

    	

        state.SetBool("IsAttacked", false);

        if (TransitionTimer > 0.0f)
            TransitionTimer -= env.pClock->FixedDeltaTime();
        else
            ai.transition = "charging_enemy_lancer";

        //std::cout << "ChaseEnemyLancer" << std::endl;
    }

    //----------------------------------------------------------------------
    GotHitEnemyLancer::GotHitEnemyLancer()
    {}
	
    void GotHitEnemyLancer::onEnter(EntityID& monster)
    {
        auto& state = env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster);
        auto& health = env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(monster);
        auto& ai = env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(monster);
        auto& rb = env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(monster);
        auto& a = env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(monster);
    	
    	if(health.CurrentHealth < 0)
    	{
            ai.transition = "idle_enemy_lancer";
            rb.isMoveable = false;
            a.AttackDelay = 10.0f;
            state.SetBool("IsAttacked", false);
            state.SetBool("BeginCharging", false);
            state.SetBool("Charging", false);
            state.SetBool("IsAlerted", false);
            state.SetBool("IsDead", true);
            return;
    	}
            

        state.SetBool("IsAttacked", true);
        state.SetBool("IsDead", false);
        TransitionTimer = 1.3f;
    }
	
    void GotHitEnemyLancer::Update(EntityID& monster)
    {
        auto& health = env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(monster);
        auto& rb = env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(monster);
        auto& ai = env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(monster);
        auto& state = env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster);
    	
        if (health.CurrentHealth < 0)
        {
            state.SetBool("IsAttacked", false);
            state.SetBool("BeginCharging", false);
            state.SetBool("Charging", false);
            state.SetBool("IsAlerted", false);
            state.SetBool("IsDead", true);
            return;
        }

        state.SetBool("IsAttacked", true);
        state.SetBool("IsDead", false);
        //auto& a = env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(monster);
        //std::cout << "GotHitEnemyLancer" << std::endl;
    	
        rb.Direction = { 0,0 };

        if (TransitionTimer > 0.0f)
            TransitionTimer -= env.pClock->FixedDeltaTime();
        else
            ai.transition = "chase_enemy_lancer";   	
    }

    void GotHitEnemyLancer::onExit(EntityID& monster)
    {
        auto& state = env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster);
        state.SetBool("IsAttacked", false);
    }
	
    //----------------------------------------------------------------------
    ChargingEnemyLancer::ChargingEnemyLancer()
    {
        TransitionEdges["hit_enemy_lancer"] = new DamagedEnemyLancer();
        // TransitionEdges["attack_enemy_lancer"] = new AttackEnemyLancer();
    }
	
    void ChargingEnemyLancer::onEnter(EntityID& monster)
    {
        auto& state = env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster);
        state.SetBool("IsAttacked", false);
        state.SetBool("BeginCharging", true);
        state.SetBool("BeginCharging", false);
        state.SetBool("Charging", true);
    }

    void ChargingEnemyLancer::Update(EntityID& monster)
    {
        auto& s = env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster);
        auto& t = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(monster);
        //auto& hp = env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(monster);
        //auto& image = env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(monster);
        auto& a = env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(monster);
        auto& rb = env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(monster);
        auto& ai = env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(monster);
        s.SetBool("IsAttacked", false);
        EntityID player = UnitManager::GetPlayerID();
        const auto player_pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(player).position;
        //auto& player_image = env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(player);
       // auto& player_col = env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(player);
        CheckEdges(monster);
        //std::cout << "ChargingEnemyLancer" << std::endl;
        //Face player
        if (AITools::EntityisOnTheRight(monster, player))
            env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(monster).m_FlipX = true;
        else
            env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(monster).m_FlipX = false;
        //If Lancer is below player, fly up first
        if (t.position.y < player_pos.y - Random::RandomFloat())
        {
            rb.Direction.y = 1;
        }
        else if (AITools::Distance_X_BetweenTwoEntities(monster, player) < 0.7f &&
            AITools::Distance_Y_BetweenTwoEntities(monster, player) < 0.5f &&
            env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(monster).MeleeCooldownTimer <= 0)
        {
            a.MeleeAttack = true;
            rb.Direction = { 0,0 };
            ai.transition = "bounce_enemy_lancer";                
        }
        //fly to player
        else
        {
            {
                AITools::FlyTowardsPoint(monster, Vector2{
                                       player_pos.x + Random::RandomFloatRange(0.1f, 0.3f),
                                       player_pos.y - Random::RandomFloatRange(0.1f, 0.3f)
                    });
            }

        }
    }

    void ChargingEnemyLancer::onExit(EntityID& monster)
    {
        //auto& state = env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster);
        //state.SetBool("Charging", false);
    }

    AttackingEnemyLancer::AttackingEnemyLancer()
    {
        TransitionEdges["hit_enemy_lancer"] = new AttackEnemyLancer();
    }
	
    void AttackingEnemyLancer::onEnter(EntityID& monster)
    {
        TransitionTimer = 0.7f;
    }

    void AttackingEnemyLancer::onExit(EntityID& monster)
    {

    }

    void AttackingEnemyLancer::Update(EntityID& monster)
    {
        //auto& a = env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(monster);
        //auto& ai = env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(monster);
        //auto& rb = env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(monster);
    	
        //std::cout << "AttackingEnemyLancer" << std::endl;

    }

    BounceEnemyLancer::BounceEnemyLancer()
    {
        original_direction = Vector2::zero();
    }
	
    void BounceEnemyLancer::onEnter(EntityID& monster)
    {
        //std::cout << "bounce enemy lancer onEnter" << std::endl;
        auto player = UnitManager::GetPlayerID();
        auto& state = env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster);
        //auto& rb = env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(monster);
    	const Vector2 player_pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(player).position;
        state.SetBool("IsBouncing", true);
        state.SetBool("Charging", false);
        TransitionTimer = 1.4f;
        env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(monster).AccumulatedForce.y += 500;
        if (AITools::EntityisOnTheRight(monster, player))
        {
            env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(monster).AccumulatedForce.x -= 9000;
        }
        else
            env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(monster).AccumulatedForce.x += 9000;
    }

    void BounceEnemyLancer::onExit(EntityID& monster)
    {
        //std::cout << "bounce enemy lancer onExit" << std::endl;
        auto& state = env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster);
        state.SetBool("IsBouncing", false);
    }
	
    void BounceEnemyLancer::Update(EntityID& monster)
    {
        //std::cout << "bounce enemy lancer Update" << std::endl;
        auto& ai = env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(monster);
        CheckEdges(monster);
        if (TransitionTimer > 0.0f)
            TransitionTimer -= env.pClock->FixedDeltaTime();
        else
            ai.transition = "chase_enemy_lancer";
    }
	
#pragma endregion

#pragma region Fiddler
    //----------------------------------------------------------------------

    IdleFiddler::IdleFiddler(Waypoint& wp, Vector2& charge_range) :
        DurationBeforeExitState{1.0f},
        waypoint{ wp }
    {
        TransitionEdges["detect_enemy_fiddler"] = new DetectEnemyFiddler(charge_range);
    }

    void IdleFiddler::onEnter(EntityID& id)
    {
        env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(id).AttackDelay = 1.0f;
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id).SetBool("IsPatrolling", true);
        DurationBeforeExitState = 1.0f;
    }

    void IdleFiddler::onExit(EntityID& id)
    {
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id).SetBool("IsPatrolling", false);
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id).SetBool("IsAlerted", true);
    }

    void IdleFiddler::Update(EntityID& monster)
    {
        auto& hp = env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(monster);
        auto& s = env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster);
        if (DurationBeforeExitState > 0.0f)
            DurationBeforeExitState -= env.pClock->FixedDeltaTime();
    	
        if (hp.isDamagedTimer <= 0.0f)
        {
            s.SetBool("IsAttacked", false);
            s.SetBool("IsAlertRunning", false);
            s.SetBool("MeleeAttack", false);
            s.SetBool("IsPatrolling", true);
            waypoint.UpdateWaypoint(monster);
            env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(monster).Direction.y = 0;
            if (DurationBeforeExitState < 0.0f)
				CheckEdges(monster);
        }
    }

    ChaseEnemyFiddler::ChaseEnemyFiddler(Vector2& lost_range)
	    : DurationBeforeExitState{1.0f},
	      FacePlayerTimer{0.8f},
		  AttackDelay{-0.1f},
		  hasAttacked{false}
    {
        TransitionEdges["lost_enemy_fiddler"] = new LostEnemyFiddler(lost_range);
    }

    void ChaseEnemyFiddler::onEnter(EntityID& id)
    {
        auto& em = env.pECS->GetWorld().GetEntityManager();
        em.GetComponent<State>(id).SetBool("IsAlerted", false);
        auto& s = em.GetComponent<State>(id);
        s.SetBool("IsPatrolling", false);
        s.SetBool("IsAlertRunning", true);
        DurationBeforeExitState = 1.0f;
        //auto& anim = em.GetComponent<Animator>(id);
   	

    }

    void ChaseEnemyFiddler::onExit(EntityID& id)
    {
        auto& s = env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id);
        env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(id).Direction = { 0, 0 };
        s.SetBool("IsAlerted", false);
        s.SetBool("IsAlertRunning", false);
        s.SetBool("IsPatrolling", true);
    }

    void ChaseEnemyFiddler::Update(EntityID& monster)
    {
        auto& em = env.pECS->GetWorld().GetEntityManager();
        auto& s = em.GetComponent<State>(monster);
        auto& a = em.GetComponent<Attack>(monster);
        auto& hp = em.GetComponent <Health> (monster);
        //auto& anim = em.GetComponent<Animator>(monster);

        if (DurationBeforeExitState > 0.0f)
            DurationBeforeExitState -= env.pClock->FixedDeltaTime();

        if (AttackDelay > 0.0f)
        {
            if (AttackDelay < 1.0f && !hasAttacked)
            {
                hasAttacked = true;
                a.MeleeAttack = true;
            }
            else if (AttackDelay < 0.2f)
            {
                s.SetBool("IsAlertRunning", true);
                s.SetBool("MeleeAttack", false);
            }
            AttackDelay -= env.pClock->FixedDeltaTime();
            return;
    	}
        hasAttacked = false;

    	if(hp.isDamagedTimer <= 0.0f)
    	{   		
            s.SetBool("IsAttacked", false);
            s.SetBool("IsDead", false);
            s.SetBool("IsAlertRunning", true);

    		if(DurationBeforeExitState < 0.0f)
				CheckEdges(monster);

            EntityID player = UnitManager::GetPlayerID();
            auto ai = env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(monster);
            auto& rb = env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(monster);
            Vector2 player_pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(player).position;

    		if(AITools::Distance_X_BetweenTwoEntities(monster, player) < 1.5f) //if too near face player immediately
    		{
                if (AITools::EntityisOnTheRight(monster, player))
                    env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(monster).m_FlipX = true;
                else
                    env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(monster).m_FlipX = false;
    		}
            else
            {
                if (FacePlayerTimer < 0.0f)
                {
                    FacePlayerTimer = 0.6f;
                    if (AITools::EntityisOnTheRight(monster, player))
                        env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(monster).m_FlipX = true;
                    else
                        env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(monster).m_FlipX = false;
                }
                else
                {
                    FacePlayerTimer -= env.pClock->FixedDeltaTime();
                }
            }

            //To Add Blocking Mechanic here

            //if (a.AttackDelay < 0.0f)
            {
                //attacking
                if (AITools::Distance_X_BetweenTwoEntities(monster, player) < 2.0f && AITools::Distance_X_BetweenTwoEntities(monster, player) > 0.5f
                    && AITools::Distance_Y_BetweenTwoEntities(monster, player) < 0.7f && a.MeleeCooldownTimer < 0.0f)
                {
                    if (AITools::EntityisOnTheRight(monster, player))
                        env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(monster).m_FlipX = true;
                    else
                        env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(monster).m_FlipX = false;
                    s.SetBool("IsAlertRunning", false);
                    s.SetBool("MeleeAttack", true);
                    rb.Direction = Vector2::zero();
                    AttackDelay = 1.6f; //time taken for attack animation to reset                	
                    hasAttacked = false;
                    return;
                }
                else if(AITools::Distance_X_BetweenTwoEntities(monster, player) < 0.5f) //fiddler is too close to player
                {
                    //std::cout << "too close" << std::endl;
                    s.SetBool("IsAlertRunning", true);
                    if (AITools::EntityisOnTheRight(player, monster))
                    {
                        rb.AccumulatedForce.x += 80;
                    }
                    else if (AITools::EntityisOnTheLeft(player, monster))
                    {
                        rb.AccumulatedForce.x -= 80;
                    }
                    return;
                }
            }
            //moving
            {
                s.SetBool("IsAlertRunning", true);
                s.SetBool("MeleeAttack", false);
                float rand_point = ai.original_point.x + Random::RandomFloatRange(-0.3f, 0.3f);
                float player_pos_right = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(player).position.x - Random::RandomFloatRange(1.5f, 2.0f);
                float player_pos_left = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(player).position.x + Random::RandomFloatRange(1.5f, 2.0f);
                if (AITools::Distance_X_BetweenEntityAndPoint(monster, ai.original_point) > 3.0f)
                {
                    AITools::MoveTowardsEntityInX(monster, rand_point);
                    rb.Direction.y = 0;
                }
                else if (AITools::EntityisOnTheRight(monster, player))
                {
                    AITools::MoveTowardsEntityInX(monster, player_pos_right);
                    rb.Direction.y = 0;
                }
                else if (AITools::EntityisOnTheLeft(monster, player))
                {
                    AITools::MoveTowardsEntityInX(monster, player_pos_left);
                    rb.Direction.y = 0;
                }

            }

    	}
        em.GetComponent<RigidBody>(monster).Direction = Vector2{ 0,0 };
        //std::cout << "here4" << std::endl;
    }
#pragma endregion

#pragma region Serpentipede
    //----------------------------------------------------------------------

    IdleSerpentipede::IdleSerpentipede(Vector2 detection_range)
    {
        TransitionEdges["detect_enemy_serpentipede"] = new DetectEnemySerpentipede(detection_range);
    }

    void IdleSerpentipede::onEnter(EntityID& id)
    {
        auto& s = env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id);
        s.SetBool("IsIdle", true);
    }

    void IdleSerpentipede::onExit(EntityID& id)
    {
        auto& s = env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id);
        s.SetBool("IsIdle", false);
    }

    void IdleSerpentipede::Update(EntityID& monster)
    {
        //std::cout << "state is idle" << std::endl;
        //std::cout << "Current anim is: " << env.pECS->GetWorld().GetEntityManager().GetComponent<Animator>(monster).m_ClipKey << std::endl;
        CheckEdges(monster);
        auto& s = env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster);
        s.SetBool("IsAlerted", false);
        s.SetBool("RangedAttack", false);
        s.SetBool("IsIdle", true);
    }

    ChaseEnemySerpentipede::ChaseEnemySerpentipede(SerpentipedeAIData& d) :
		Attacking {false},
        CooldownTimer{ 1.0f },
        BurrowDownDuration{ 2.0f },
        BurrowUpDuration{ 1.0f },
		BurrowDownDelay{1.2f},
        BurrowState{ 0 },
        CurrentPoint{ 0 },
        SerpentData{ d }
    {
        TransitionEdges["lost_enemy_serpentipede"] = new LostEnemySerpentipede(SerpentData.DetectionRange);
    }

    void ChaseEnemySerpentipede::onEnter(EntityID& id)
    {
        auto& s = env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id);
        s.SetBool("IsAlerted", true);
    }

    void ChaseEnemySerpentipede::onExit(EntityID& id)
    {
        env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(id).Direction = { 0, 0 };
        auto& s = env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id);
        s.SetBool("IsAlerted", false);
        s.SetBool("IsIdle", true);
    }

    void ChaseEnemySerpentipede::Update(EntityID& monster)
    {
        auto& em = env.pECS->GetWorld().GetEntityManager();
        auto ai = em.GetComponent<AI>(monster);
        auto& rb = em.GetComponent<RigidBody>(monster);
        auto& s = em.GetComponent<State>(monster);
        auto& a = em.GetComponent<Attack>(monster);
        auto& collider = em.GetComponent<Collider>(monster);
        auto& rend = em.GetComponent<Renderer2D>(monster);
        auto& hp = em.GetComponent<Health>(monster);
        //std::cout << "burrow state is: " << BurrowState << std::endl;
        //std::cout << "state is chase" << std::endl;
        //std::cout << "Current anim is: " << env.pECS->GetWorld().GetEntityManager().GetComponent<Animator>(monster).m_ClipKey << std::endl;

        if (hp.isDamagedTimer > 0.0f || a.AttackDelay > 0.0f || hp.CurrentHealth < 0.0f)
        {
            return;
        }

        s.SetBool("IsDead", false);
        //Seen and attacking
        if (BurrowState == 0 && !Attacking)
        {
            s.SetBool("IsIdle", false);
            rb.Direction = Vector2::zero();
            if (CooldownTimer <= 0)
            {
                EntityID player = UnitManager::GetPlayerID();
                if (AITools::EntityisOnTheRight(monster, player))
                    env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(monster).m_FlipX = true;
                else
                    env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(monster).m_FlipX = false;

                const float distX = AITools::Distance_X_BetweenTwoEntities(monster, player);
                s.SetBool("IsAlerted", true);
                if(distX < 1.5f)
                { //Burrow instead of attacking
                    s.SetBool("RangedAttack", false);
                    s.SetBool("IsAlerted", false);
                    s.SetBool("IsBurrowing", true);
                    BurrowDownDelay = 0.6f;
                    BurrowState = 1;
                    return;
                }
            	
                if (distX < 6.0f
                    && AITools::Distance_Y_BetweenTwoEntities(monster, player) < 2.5f)
                { //Attack since distance is far enough
                    a.RangeAttack = true;
                    s.SetBool("RangedAttack", true);
                    a.AttackDelay = 0.8f;
                    CooldownTimer = 1.0f;
                	BurrowDownDelay = 0.6f;
                    Attacking = true;
                }
                else
                {
                    CheckEdges(monster);
                	if (Random::RandomIntRange(1,15) < 2)
                	{
                        s.SetBool("RangedAttack", false);
                        s.SetBool("IsAlerted", false);
                        s.SetBool("IsBurrowing", true);
                        BurrowDownDelay = 0.6f;
                        BurrowState = 1;
                        return;
                	}
                }

            }
            else
            {
                CooldownTimer -= env.pClock->FixedDeltaTime();
            }
            return;
        }

        if (a.AttackDelay < 0.0f && s.GetBool("RangedAttack"))
        {
            s.SetBool("RangedAttack", false);
            s.SetBool("IsAlerted", false);
            s.SetBool("IsBurrowing", true);
            BurrowDownDelay = 1.2f;
            Attacking = false;
            BurrowState = 1;
        }

        if (BurrowDownDelay > 0.0f)
        {
            BurrowDownDelay -= env.pClock->FixedDeltaTime();
            return;
        }

    	
        //Burrowing Down
        if (BurrowState == 1)
        {
            if (BurrowDownDuration > 0.0f)
            {
                BurrowDownDuration -= env.pClock->FixedDeltaTime();
                s.SetBool("IsAlerted", true);
            }
            else
            {
                s.SetBool("IsBurrowing", false);
                s.SetBool("IsAlerted", false);
                s.SetBool("MoveBurrowing", true);
                BurrowState = 2;
                auto& renderer = env.pECS->GetWorld().GetEntityManager().GetComponent<Renderer2D>(monster);
                auto& health = env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(monster);
                health.isInvulnerable = true;
                rb.isMoveable = true;
                renderer.m_Active = true;
                collider.isTrigger = true;
                rb.hasGravity = false;
                rb.Movespeed *= 1.2f;
                rend.m_SortingLayer = 2;
                BurrowDownDuration = 0.7f;
                switch (CurrentPoint)
                {
                case 0:
                    CurrentPoint = Random::RandomIntRange(1, 3); //random from 1, 2
                    break;
                case 1:
                    if (Random::RandomIntRange(1, 3) == 1)
                        CurrentPoint = 0;
                    else
                        CurrentPoint = 2;
                    break;
                case 2:
                    CurrentPoint = Random::RandomIntRange(0, 2); //random from 1, 2
                    break;
                default:
                    return;
                }
            }
            return;
        }

        //Hidden
        if (BurrowState == 2)
        {
            //auto current_pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(monster).position;
            s.SetBool("IsAlerted", false);
            s.SetBool("MoveBurrowing", true);
            //Move towards next point
            if (AITools::EntityisAtPointInX(monster, ai.original_point.x + SerpentData.Points[CurrentPoint].x, 0.2f))
            {
                //Monster is at point
                BurrowState = 3;
                rb.Direction = Vector2::zero();
                rb.isMoveable = false;
                s.SetBool("IsUnborrowing", true);
                s.SetBool("MoveBurrowing", false);
            }
            Vector2 move_point = ai.original_point + SerpentData.Points[CurrentPoint];
            AITools::MoveTowardsPoint(monster, move_point);
            rb.Direction.y = 0;
            return;
        }

        //Burrowing Up
        if (BurrowState == 3)
        {
            rb.Direction = Vector2::zero();
            if (BurrowUpDuration > 0.0f)
            {
                BurrowUpDuration -= env.pClock->FixedDeltaTime();
                env.pECS->GetWorld().GetEntityManager().GetComponent<Renderer2D>(monster).m_Active = true;
                s.SetBool("IsUnborrowing", true);
            }
            else
            {
                s.SetBool("IsUnborrowing", false);
                s.SetBool("IsAlerted", true);
                BurrowState = 0;
                BurrowUpDuration = 1.0f;
                collider.isTrigger = false;
                rb.isMoveable = true;
                rb.hasGravity = true;
                rb.Movespeed /= 1.2f;
                rend.m_SortingLayer = 1;
                auto& health = env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(monster);
                health.isInvulnerable = false;
                CooldownTimer = 1.0f;


                collider.CollisionLayerCheck = 9;
            }
        }


    }
#pragma endregion
    LancerAIData::LancerAIData() :
        ChargeDetectionRange{ Vector2::zero() }
    {
    }

    LancerAIData::LancerAIData(LancerAIData& d) :
        ChargeDetectionRange{ d.ChargeDetectionRange }
    {
    }

    FiddlerAIData::FiddlerAIData() :
        waypoint{},
        ChargeDetectionRange{ Vector2::zero() },
        LostDetectionRange{ Vector2::zero() }
    {
    }

    FiddlerAIData::FiddlerAIData(FiddlerAIData& d) :
        waypoint{ d.waypoint },
        ChargeDetectionRange{ d.ChargeDetectionRange },
        LostDetectionRange{ d.LostDetectionRange }
    {
    }

    SerpentipedeAIData::SerpentipedeAIData() :
        AttackCooldown{ 0.0f },
        Points{ Vector2::zero(), Vector2::zero(), Vector2::zero() },
        DetectionRange{ Vector2::zero() }
    {
    }

    SerpentipedeAIData::SerpentipedeAIData(SerpentipedeAIData& d) :
        AttackCooldown{ d.AttackCooldown },
        Points{ d.Points[0], d.Points[1], d.Points[2] },
        DetectionRange{ d.DetectionRange }
    {
    }
}
