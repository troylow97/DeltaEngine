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
  }

  void IdleLancer::onExit(EntityID& id)
  {
  }

  void IdleLancer::Update(EntityID& monster)
  {
    CheckEdges(monster);
    auto& ref = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(monster).position;

    //if (ref.y < 2.0)
    //    AITools::MoveTowardsPoint(monster, Vector2{ ref.x,Random::RandomFloatRange(2.1,2.5) });
  }

  //----------------------------------------------------------------------
  ChaseEnemyLancer::ChaseEnemyLancer()
  {
  }

  void ChaseEnemyLancer::onEnter(EntityID& id)
  {
  }

  void ChaseEnemyLancer::onExit(EntityID& id)
  {
    env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(id).Direction = {0, 0};
  }

  void ChaseEnemyLancer::Update(EntityID& monster)
  {
    //CheckEdges(monster); lancer continues chasing so no transition edge
    EntityID player = UnitManager::GetPlayerID();
    const auto player_pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(player).position;

  	//Move towards player
    AITools::FlyTowardsPoint(monster, Vector2{
                               player_pos.x + Random::RandomFloatRange(-0.3f, 0.3f),
                               player_pos.y + Random::RandomFloatRange(-0.2f, 0.4f)
                             });

    //Attack if player  is close AND attack cooldown is 0
    if (AITools::Distance_X_BetweenTwoEntities(monster, player) < 0.3f &&
      AITools::Distance_Y_BetweenTwoEntities(monster, player) < 1 && env.pECS->GetWorld().GetEntityManager().
                                                                         GetComponent<Attack>(monster).
                                                                         MeleeCooldownTimer <= 0)
    {
      AITools::FaceEntity(monster, player);
      env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(monster).MeleeAttack = true;
      //if (Random::RandomFloatRange(0, 100) < 20)
      {
          static size_t c_id{ u64_max };
          if (AudioEngine::IsChannelPlaying(c_id))
              AudioEngine::StopChannel(c_id);
          c_id = AudioEngine::Play("Audio/Lancer/LancerBuzz.ogg");
      }
    }


  }

  //----------------------------------------------------------------------

  IdleFiddler::IdleFiddler(Waypoint& wp, Vector2& charge_range) :
    waypoint{wp}
  {
    TransitionEdges["detect_enemy_fiddler"] = new DetectEnemyFiddler(charge_range);
  }

  void IdleFiddler::onEnter(EntityID& id)
  {
  }

  void IdleFiddler::onExit(EntityID& id)
  {
    env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id).SetBool("IsPatrolling", false);
    env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id).SetBool("IsAlerted", true);
  }

  void IdleFiddler::Update(EntityID& monster)
  {
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
  }

  void ChaseEnemyFiddler::onExit(EntityID& id)
  {
    env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(id).Direction = {0, 0};
    env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id).SetBool("IsAlertRunning", false);
  }

  void ChaseEnemyFiddler::Update(EntityID& monster)
  {
      env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster).SetBool("IsAlerted", false);
    env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster).SetBool("IsAlertRunning", true);
    CheckEdges(monster);
    EntityID player = UnitManager::GetPlayerID();
    auto& ref = env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(monster);
    if (AITools::Distance_X_BetweenTwoEntities(monster, player) < 2 && 
        env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(monster).MeleeCooldownTimer <= 0)
    {
      AITools::FaceEntity(monster, player);
      env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster).SetBool("IsAlertRunning", false);
      env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(monster).MeleeAttack = true;
      std::cout << "melee attacking" << std::endl;
    }

  	if(AITools::Distance_X_BetweenEntityAndPoint(monster, ref.original_point) < 1)
		AITools::MoveTowardsEntityInX(monster, player);
    else
        AITools::MoveTowardsPoint(monster, ref.original_point);
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
    CooldownTimer{1.0f},
	BurrowDownDuration{0.5f},
    BurrowUpDuration{ 0.5f },
    BurrowState{0},
    CurrentPoint{0},
    SerpentData{d}
  {
    TransitionEdges["lost_enemy_serpentipede"] = new LostEnemySerpentipede(SerpentData.DetectionRange);
  }

  void ChaseEnemySerpentipede::onEnter(EntityID& id)
  {
  }

  void ChaseEnemySerpentipede::onExit(EntityID& id)
  {
    env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(id).Direction = {0, 0};
    env.pECS->GetWorld().GetEntityManager().GetComponent<State>(id).SetBool("PlayerDetected", false);
  }

  void ChaseEnemySerpentipede::Update(EntityID& monster)
  {
    CheckEdges(monster);
    auto ai = env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(monster);
    const auto trans = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(monster);
    auto& rb = env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(monster);
    auto& s = env.pECS->GetWorld().GetEntityManager().GetComponent<State>(monster);
    //const Vector2 pos = ai.original_point;
    //
    //std::cout << "Burrow state is: " << BurrowState << std::endl;
    //std::cout << "Moving to: " << pos.x + SerpentData.Points[CurrentPoint].x << std::endl;
    //std::cout << "Current point: " << trans.position.x << std::endl;
    //std::cout << "Direction is " << rb.Direction.x << std::endl;
    //
    //std::cout << "points " << 0 << " is " << pos.x + SerpentData.Points[0].x << "," << pos.y + SerpentData.Points[0].y << std::endl;
    //std::cout << "points " << 1 << " is " << pos.x + SerpentData.Points[1].x << "," << pos.y + SerpentData.Points[1].y << std::endl;
    //std::cout << "points " << 2 << " is " << pos.x + SerpentData.Points[2].x << "," << pos.y + SerpentData.Points[2].y << std::endl;
    //s.SetBool("PlayerDetected", true);
  	
  	//Seen and attacking
    if(BurrowState == 0)
    {
        rb.Direction = Vector2::zero();
        if (CooldownTimer <= 0)
        {
            AITools::FaceEntity(monster, UnitManager::GetPlayerID());
            auto& attack = env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(monster);
            attack.RangeAttack = true;
            //Burrow Serpentipede
            BurrowState = 1;
            CooldownTimer = 1.0f;
        }
        else
        {
            CooldownTimer -= env.pClock->FixedDeltaTime();
        }
        return;
    }

  	//Burrowing Down
  	if(BurrowState == 1)
  	{
        if (BurrowDownDuration > 0.0f)
        {
            BurrowDownDuration -= env.pClock->FixedDeltaTime();
        	//Add burrow down animation
        }

        else
        {
            BurrowState = 2;
            auto& renderer = env.pECS->GetWorld().GetEntityManager().GetComponent<Renderer2D>(monster);
            auto& health = env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(monster);
            auto& collider = env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(monster);
            health.isInvulnerable = true;
            renderer.m_Active = false;
            collider.CollisionLayerCheck = 1;
            BurrowDownDuration = 0.5f;
        	switch(CurrentPoint)
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
        auto ai = env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(monster);
        //auto current_pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(monster).position;
    	
        //Move towards next point
        if (AITools::EntityisAtPointInX(monster, ai.original_point.x + SerpentData.Points[CurrentPoint].x, 0.2f))
        {
            //Monster is at point
            BurrowState = 3;
            rb.Direction = Vector2::zero();
        }
        AITools::MoveTowardsPoint(monster, ai.original_point + SerpentData.Points[CurrentPoint]);
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
        	//Play burrowing up animation
            env.pECS->GetWorld().GetEntityManager().GetComponent<Renderer2D>(monster).m_Active = true;
        }
        else
        {
            BurrowState = 0;
            BurrowUpDuration = 0.5f;
            auto& health = env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(monster);
            auto& collider = env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(monster);
            health.isInvulnerable = false;
            CooldownTimer = 3.0f;


            collider.CollisionLayerCheck = 9;
        }
    }

  	
  }

  LancerAIData::LancerAIData() :
    ChargeDetectionRange{Vector2::zero()}
  {
  }

  LancerAIData::LancerAIData(LancerAIData& d) :
    ChargeDetectionRange{d.ChargeDetectionRange}
  {
  }

  FiddlerAIData::FiddlerAIData() :
    waypoint{},
    ChargeDetectionRange{Vector2::zero()},
    LostDetectionRange{Vector2::zero()}
  {
  }

  FiddlerAIData::FiddlerAIData(FiddlerAIData& d) :
    waypoint{d.waypoint},
    ChargeDetectionRange{d.ChargeDetectionRange},
    LostDetectionRange{d.LostDetectionRange}
  {
  }

  SerpentipedeAIData::SerpentipedeAIData() :
    AttackCooldown{0.0f},
    Points{Vector2::zero(), Vector2::zero(), Vector2::zero()},
    DetectionRange{Vector2::zero()}
  {
  }

  SerpentipedeAIData::SerpentipedeAIData(SerpentipedeAIData& d) :
    AttackCooldown{d.AttackCooldown},
    Points{d.Points[0], d.Points[1], d.Points[2]},
    DetectionRange{d.DetectionRange}
  {
  }
}
