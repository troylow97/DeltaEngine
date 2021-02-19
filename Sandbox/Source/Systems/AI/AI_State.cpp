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
        Bouncing{ false },
        BouncingTimer{ 0.7f }
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
        bool attacking = false;
        auto& a = env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(monster);
    	
        //Check distance
        if(AITools::Distance_X_BetweenTwoEntities(monster, player) > 0.8f)
        {
            s.SetBool("IsAlerted", false);
            s.SetBool("LancerCharge", true);
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
        else if(AITools::Distance_Y_BetweenTwoEntities(monster, player) < 0.5f && //Attack if near enough
                t.position.y > player_pos.y && 
                env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(monster).MeleeCooldownTimer <= 0 && !Bouncing) 

        {
            if (AITools::EntityisOnTheRight(monster, player))
                env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(monster).m_FlipX = true;
            else
                env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(monster).m_FlipX = false;
            a.MeleeAttack = true;
            s.SetBool("LancerCharge", false);
            s.SetBool("MeleeAttack", true);	        
        }
        else
        {
            //Check if attacking trigger bounce
            if (!Bouncing && a.MeleeCooldownTimer > 0.3f && a.MeleeCooldownTimer < 0.8f)
            {
                s.SetBool("IsBouncing", true);
                s.SetBool("MeleeAttack", false);
                Bouncing = true;
            }

            if (Bouncing && BouncingTimer > 0.0f)
                BouncingTimer -= env.pClock->FixedDeltaTime();
            else
            {
                BouncingTimer = 0.7f;
                Bouncing = false;
                s.SetBool("IsBouncing", false);
                s.SetBool("IsAlerted", true);
            }
        	


        	
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
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id).SetBool("IsPatrolling", true);
    }

    void IdleFiddler::onExit(EntityID& id)
    {
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id).SetBool("IsPatrolling", false);
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id).SetBool("IsAlerted", true);
    }

    void IdleFiddler::Update(EntityID& monster)
    {
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster).SetBool("IsAlertRunning", false);
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster).SetBool("MeleeAttack", false);
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster).SetBool("IsPatrolling", true);
        waypoint.UpdateWaypoint(monster);
        CheckEdges(monster);
    }

    ChaseEnemyFiddler::ChaseEnemyFiddler(Vector2& lost_range)
    {
        TransitionEdges["lost_enemy_fiddler"] = new LostEnemyFiddler(lost_range);
    }

    void ChaseEnemyFiddler::onEnter(EntityID& id)
    {
        env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id).SetBool("IsAlerted", false);
        auto& s = env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id);
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
        s.SetBool("IsPatrolling", false);
        CheckEdges(monster);

        EntityID player = UnitManager::GetPlayerID();
        auto& ref = env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(monster);
        //To Add Blocking Mechanic here


        if (AITools::Distance_X_BetweenTwoEntities(monster, player) < 2.5f && AITools::Distance_Y_BetweenTwoEntities(monster, player) < 1.5f &&
            !a.MeleeAttack && a.MeleeCooldownTimer <= 0 && a.AttackDelay < 0.0f)
        {
            if (AITools::EntityisOnTheRight(monster, player))
            {
                env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(monster).m_FlipX = true;
            }
            else
            {
                env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(monster).m_FlipX = false;
            }
            s.SetBool("IsAlertRunning", false);
            s.SetBool("MeleeAttack", true);
            a.MeleeAttack = true;
            a.AttackDelay = 0.8f;
            return;
        }

    	if(a.AttackDelay < 0.0f)
    	{
            s.SetBool("IsAlertRunning", true);
            s.SetBool("MeleeAttack", false);
    	}

        if (s.GetBool("MeleeAttack") == false && a.MeleeAttack && a.MeleeCooldownTimer <= 0)
        {
            s.SetBool("IsAlertRunning", true);
            if (AITools::Distance_X_BetweenEntityAndPoint(monster, ref.original_point) < 1)
                AITools::MoveTowardsEntityInX(monster, player);
            else
                AITools::MoveTowardsPoint(monster, ref.original_point);
        }

    }

    //----------------------------------------------------------------------

    IdleSerpentipede::IdleSerpentipede(Vector2 detection_range)
    {
        TransitionEdges["detect_enemy_serpentipede"] = new DetectEnemySerpentipede(detection_range);
    }

    void IdleSerpentipede::onEnter(EntityID& id)
    {
    }

    void IdleSerpentipede::onExit(EntityID& id)
    {
    }

    void IdleSerpentipede::Update(EntityID& monster)
    {
        CheckEdges(monster);

    }

    ChaseEnemySerpentipede::ChaseEnemySerpentipede(SerpentipedeAIData& d) :
        CooldownTimer{ 1.0f },
        BurrowDownDuration{ 0.5f },
        BurrowUpDuration{ 0.5f },
        BurrowState{ 0 },
        CurrentPoint{ 0 },
        SerpentData{ d }
    {
        TransitionEdges["lost_enemy_serpentipede"] = new LostEnemySerpentipede(SerpentData.DetectionRange);
    }

    void ChaseEnemySerpentipede::onEnter(EntityID& id)
    {
    }

    void ChaseEnemySerpentipede::onExit(EntityID& id)
    {
        env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(id).Direction = { 0, 0 };
    }

    void ChaseEnemySerpentipede::Update(EntityID& monster)
    {
        CheckEdges(monster);
        auto ai = env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(monster);
        auto& rb = env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(monster);
        auto& s = env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster);
        auto& a = env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(monster);
        auto& collider = env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(monster);
        auto& rend = env.pECS->GetWorld().GetEntityManager().GetComponent<Renderer2D>(monster);
        //Seen and attacking
        if (BurrowState == 0)
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

                a.RangeAttack = true;
                s.SetBool("RangedAttack", true);
                a.AttackDelay = 0.2f;
                BurrowState = 1;
                CooldownTimer = 1.0f;
            }
            else
            {
                CooldownTimer -= env.pClock->FixedDeltaTime();
            }
            return;
        }

        if (a.AttackDelay < 0.0f)
        {
            s.SetBool("IsBurrowing", true);
            s.SetBool("RangedAttack", false);
        }
        else
        {
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
                s.SetBool("MoveBurrowing", true);
                BurrowState = 2;
                auto& renderer = env.pECS->GetWorld().GetEntityManager().GetComponent<Renderer2D>(monster);
                auto& health = env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(monster);
                health.isInvulnerable = true;
                renderer.m_Active = false;
                collider.isTrigger = true;
                rb.hasGravity = false;
                rb.Movespeed *= 1.2f;
                rend.m_SortingLayer = 2;
                BurrowDownDuration = 0.5f;
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

            //Move towards next point
            if (AITools::EntityisAtPointInX(monster, ai.original_point.x + SerpentData.Points[CurrentPoint].x, 0.2f))
            {
                //Monster is at point
                BurrowState = 3;
                rb.Direction = Vector2::zero();
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
                rb.hasGravity = true;
                rb.Movespeed /= 1.2f;
                rend.m_SortingLayer = 1;
                auto& health = env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(monster);
                health.isInvulnerable = false;
                CooldownTimer = 3.0f;


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
