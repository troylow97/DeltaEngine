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
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id).SetBool("IsIdle", true);
    }

    void IdleLancer::onExit(EntityID& id)
    {
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id).SetBool("IsIdle", false);
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id).SetBool("IsAlerted", true);
    }

    void IdleLancer::Update(EntityID& monster)
    {
        CheckEdges(monster);
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster).SetBool("IsAlerted", true);
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster).SetBool("MeleeAttack", false);

        //auto& ref = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(monster).position;

        //if (ref.y < 2.0)
        //    AITools::MoveTowardsPoint(monster, Vector2{ ref.x,Random::RandomFloatRange(2.1,2.5) });
    }

    //----------------------------------------------------------------------
    ChaseEnemyLancer::ChaseEnemyLancer() :
		Charging {false },
        Bouncing{ false },
        BouncingTimer{ 0.7f },
		ChargeTimer{ 0.0f}
    {
    }

    void ChaseEnemyLancer::onEnter(EntityID& id)
    {

    }

    void ChaseEnemyLancer::onExit(EntityID& id)
    {
        env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(id).Direction = { 0, 0 };

    }

    void ChaseEnemyLancer::Update(EntityID& monster)
    {
        //CheckEdges(monster); lancer continues chasing so no transition edge
        EntityID player = UnitManager::GetPlayerID();
        auto& s = env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster);
        auto& t = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(monster);
        const auto player_pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(player).position;
        auto& player_image = env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(player);
        auto& player_col = env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(player);
        auto& animator = env.pECS->GetWorld().GetEntityManager().GetComponent<Animator>(monster);
        bool attacking = false;
        auto& a = env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(monster);

        if (BouncingTimer > 0.0f)
        {
            if(!Bouncing && BouncingTimer < 0.7f)
            {
                Bouncing = false;
                s.SetBool("IsBouncing", false);
                s.SetBool("IsAlerted", true);
            }
            BouncingTimer -= env.pClock->FixedDeltaTime();
            return;
        }
   	
        //Check distance
        if(AITools::Distance_X_BetweenTwoEntities(monster, player) > 0.8f && !a.MeleeAttack)
        {
        	if(!Charging)
        	{
                s.SetBool("IsAlerted", false);
                s.SetBool("MeleeAttack", true);
                Charging = true;
                ChargeTimer = 0.8f;
        	}
        	
        	if(Charging && s.GetBool("MeleeAttack"))
        	{
        		if(ChargeTimer < 0.0f)
        		{
                    s.SetBool("MeleeAttack", false);
                    s.SetBool("LancerCharge", true);
        		}
                else
                {
                    ChargeTimer -= env.pClock->FixedDeltaTime();
                }
        	}

            //Face player
            if (AITools::EntityisOnTheRight(monster, player))
                env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(monster).m_FlipX = true;
            else
                env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(monster).m_FlipX = false;
            //Move towards player
            if (player_image.m_FlipX == false)
            {
                AITools::FlyTowardsPoint(monster, Vector2{
                                           player_pos.x + Random::RandomFloatRange(0.5f, 0.3f),
                                           player_pos.y + Random::RandomFloatRange(-0.1f, 0.0f)
                    });
            }
            else
            {
                AITools::FlyTowardsPoint(monster, Vector2{
                                           player_pos.x + Random::RandomFloatRange(-0.5f, 0.3f),
                                           player_pos.y + Random::RandomFloatRange(-0.1f, 0.0f)
                    });
            }
      
        }
    	//Start Attacking
        else if(AITools::Distance_Y_BetweenTwoEntities(monster, player) < 0.3f &&
                env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(monster).MeleeCooldownTimer <= 0)
        {
            if (AITools::EntityisOnTheRight(monster, player))
                env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(monster).m_FlipX = true;
            else
                env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(monster).m_FlipX = false;
            s.SetBool("LancerCharge", false);
            s.SetBool("MeleeAttack", false);
            s.SetBool("IsBouncing", true);
            Charging = false;
            Vector2 monster_pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(monster).position;
            Vector2 kb = (player_pos - env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(monster).position);

            //Apply knockback to lancer
            env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(monster).AccumulatedForce += -kb.Normalize() * 4000.0f;
            BouncingTimer = 1.5f;
        }

    }

    //----------------------------------------------------------------------

    IdleFiddler::IdleFiddler(Waypoint& wp, Vector2& charge_range) :
        waypoint{ wp }
    {
        TransitionEdges["detect_enemy_fiddler"] = new DetectEnemyFiddler(charge_range);
    }

    void IdleFiddler::onEnter(EntityID& id)
    {
        env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(id).AttackDelay = 1.0f;
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id).SetBool("IsPatrolling", true);
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
        if (hp.isDamagedTimer <= 0.0f)
        {
            s.SetBool("IsAttacked", false);
            s.SetBool("IsAlertRunning", false);
            s.SetBool("MeleeAttack", false);
            s.SetBool("IsPatrolling", true);
            waypoint.UpdateWaypoint(monster);
            CheckEdges(monster);
        }
    }

    ChaseEnemyFiddler::ChaseEnemyFiddler(Vector2& lost_range)
	    : Attacking{false}
    {
        TransitionEdges["lost_enemy_fiddler"] = new LostEnemyFiddler(lost_range);
    }

    void ChaseEnemyFiddler::onEnter(EntityID& id)
    {
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id).SetBool("IsAlerted", false);
        auto& s = env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id);
        s.SetBool("IsPatrolling", false);
        s.SetBool("IsAlertRunning", true);

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
        std::cout << "clip is: " << em.GetComponent<Animator>(monster).m_ClipKey << std::endl;
    	if(hp.isDamagedTimer <= 0.0f)
    	{
            CheckEdges(monster);

            EntityID player = UnitManager::GetPlayerID();
            auto ai = env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(monster);
            auto& rb = env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(monster);
            Vector2 player_pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(player).position;

            if (AITools::EntityisOnTheRight(monster, player))
                env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(monster).m_FlipX = true;
            else
                env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(monster).m_FlipX = false;

            //To Add Blocking Mechanic here

            if (a.AttackDelay < 0.0f)
            {
                //attacking
                if (AITools::Distance_X_BetweenTwoEntities(monster, player) < 2.5f && AITools::Distance_X_BetweenTwoEntities(monster, player) > 0.5f
                    && AITools::Distance_Y_BetweenTwoEntities(monster, player) < 1.5f &&
                    a.MeleeCooldownTimer <= 0 && Attacking == false)
                {
                    s.SetBool("IsAttacked", false);
                    s.SetBool("IsAlerted", true);
                    s.SetBool("IsAlerted", false);
                    s.SetBool("IsAlertRunning", true);
                    s.SetBool("IsAlertRunning", false);
                    s.SetBool("MeleeAttack", true);
                    rb.Direction = Vector2::zero();
                    a.AttackDelay = 1.0f;
                    Attacking = true;
                    std::cout << "here1" << std::endl;
                    return;
                }

                //moving
                {
                    std::cout << "here2" << std::endl;
                    s.SetBool("IsAttacked", false);
                    s.SetBool("IsAlerted", true);
                    s.SetBool("IsAlerted", false);
                    s.SetBool("IsAlertRunning", true);
                    s.SetBool("MeleeAttack", false);
                    float rand_point = ai.original_point.x + Random::RandomFloatRange(-0.3f, 0.3f);
                    float player_pos_right = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(player).position.x - Random::RandomFloatRange(1.5f, 2.0f);
                    float player_pos_left = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(player).position.x + Random::RandomFloatRange(1.5f, 2.0f);
                    if (AITools::Distance_X_BetweenEntityAndPoint(monster, ai.original_point) > 3.0f)
                    {
                        AITools::MoveTowardsEntityInX(monster, rand_point);
                    }
                    else if (AITools::EntityisOnTheRight(monster, player))
                    {
                        AITools::MoveTowardsEntityInX(monster, player_pos_right);
                    }
                    else if (AITools::EntityisOnTheLeft(monster, player))
                    {
                        AITools::MoveTowardsEntityInX(monster, player_pos_left);
                    }

                }
            }

            if (Attacking && a.AttackDelay < 0.0f)
            {
                Attacking = false;
                a.MeleeAttack = true;
            }
            std::cout << "here3" << std::endl;
    	}
        em.GetComponent<RigidBody>(monster).Direction = Vector2{ 0,0 };
        std::cout << "here4" << std::endl;
    }

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
        CheckEdges(monster);

    }

    ChaseEnemySerpentipede::ChaseEnemySerpentipede(SerpentipedeAIData& d) :
		Attacking {false},
        CooldownTimer{ 1.0f },
        BurrowDownDuration{ 1.5f },
        BurrowUpDuration{ 0.5f },
		BurrowDownDelay{0.4f},
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
        auto ai = env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(monster);
        auto& rb = env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(monster);
        auto& s = env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster);
        auto& a = env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(monster);
        auto& collider = env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(monster);
        auto& rend = env.pECS->GetWorld().GetEntityManager().GetComponent<Renderer2D>(monster);
        std::cout << "burrow state is: " << BurrowState << std::endl;
        std::cout << "Current anim is: " << env.pECS->GetWorld().GetEntityManager().GetComponent<Animator>(monster).m_ClipKey << std::endl;
        //Seen and attacking
        if (BurrowState == 0 && !Attacking)
        {
            rb.Direction = Vector2::zero();
            if (CooldownTimer <= 0)
            {
                EntityID player = UnitManager::GetPlayerID();
                if (AITools::EntityisOnTheRight(monster, player))
                {
                    env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(monster).m_FlipX = true;
                }
                else
                {
                    env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(monster).m_FlipX = false;
                }
                const float distX = AITools::Distance_X_BetweenTwoEntities(monster, player);
            	
                if(distX < 1.5f)
                {
                    s.SetBool("RangedAttack", false);
                    s.SetBool("IsAlerted", false);
                    s.SetBool("IsBurrowing", true);
                    BurrowDownDelay = 0.4f;
                    BurrowState = 1;
                    return;
                }
            	
                if (distX < 7.5f
                    && AITools::Distance_Y_BetweenTwoEntities(monster, player) < 3.0f)
                {
                    a.RangeAttack = true;
                    s.SetBool("RangedAttack", true);
                    a.AttackDelay = 1.5f;
                    CooldownTimer = 1.0f;
                    Attacking = true;
                }
                else
                {
                    CheckEdges(monster);                	
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
            BurrowDownDelay = 0.4f;
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
                BurrowDownDuration = 1.5f;
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
            }
            else
            {
                s.SetBool("IsUnborrowing", false);
                s.SetBool("IsAlerted", true);
                BurrowState = 0;
                BurrowUpDuration = 0.5f;
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
