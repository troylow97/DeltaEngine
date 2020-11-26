#include "AI_State.h"
#include "AI_Transition.h"
#include "Core/Utils/Random.h"
#include "Core/GlobalStruct.h"
#include "Core/GameClock/GameClock.h"

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
    env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& player, EntityType& et)
    {
      if (et.type == EntityCategory::E_PLAYER)
      {
        auto player_pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(player).position;
        auto player_size = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(player).scale;
        AITools::FlyTowardsPoint(monster, Vector2{player_pos.x + Random::RandomFloatRange(-0.3,0.3),player_pos.y + Random::RandomFloatRange(0.5,1.5)});
      }
    });
  }

  //----------------------------------------------------------------------

  IdleFiddler::IdleFiddler(Waypoint& wp,Vector2& charge_range) :
      waypoint{wp}
  {
    TransitionEdges["detect_enemy_fiddler"] = new DetectEnemyFiddler(charge_range);
  }

  void IdleFiddler::onEnter(EntityID& id)
  {
  }

  void IdleFiddler::onExit(EntityID& id)
  {
  }

  void IdleFiddler::Update(EntityID& monster)
  {
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
  }

  void ChaseEnemyFiddler::Update(EntityID& monster)
  {
    CheckEdges(monster);
    env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& player, EntityType& et)
    {
      if (et.type == EntityCategory::E_PLAYER)
      {
          if (AITools::Distance_X_BetweenTwoEntities(monster, player) < 2 && env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(monster).CooldownTimer <= 0)
          {
              AITools::FaceEntity(monster, player);
              env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(monster).MeleeAttack = true;
          }
          AITools::MoveTowardsEntityInX(monster, player);
      }
    });
  }

  //----------------------------------------------------------------------

  IdleSerpentipede::IdleSerpentipede(Vector2 startpoint,Vector2 detection_range)
  {
      TransitionEdges["detect_enemy_serpentipede"] = new DetectEnemySerpentipede(startpoint, detection_range);
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

  ChaseEnemySerpentipede::ChaseEnemySerpentipede(SerpentipedeData& d) :
      SerpentData{d},
      CooldownTimer{0.0f},
      CurrentPoint{0}
  {
      TransitionEdges["lost_enemy_serpentipede"] = new LostEnemySerpentipede(SerpentData.Points[0], SerpentData.DetectionRange);
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
        
      if (CooldownTimer <= 0)
      {
          auto& attack = env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(monster);
          auto& ai = env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(monster);

          if (AITools::EntityisAtPoint(monster, ai.original_point + SerpentData.Points[CurrentPoint]))
          {
              env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& player, EntityType& et)
                  {
                      if (et.type == EntityCategory::E_PLAYER)
                      {
                            if (attack.CooldownTimer <= 0)
                            {
                                CurrentPoint = Random::RandomIntRange(0, 3);
                                AITools::FaceEntity(monster, player);
                                attack.RangeAttack = true;
                                CooldownTimer = SerpentData.MaxCooldown;
                            }
                      }
                  });
          }
          else
          {
              AITools::MoveTowardsPoint(monster, ai.original_point + SerpentData.Points[CurrentPoint]);
          }
      }
      else
      {
          CooldownTimer -= env.pClock->DeltaTime();
      }

  }

  LancerData::LancerData() :
      ChargeDetectionRange{Vector2::zero()}
  {}

  LancerData::LancerData(LancerData& d) :
      ChargeDetectionRange{ d.ChargeDetectionRange }
  {}

  FiddlerData::FiddlerData() :
      waypoint{},
      ChargeDetectionRange{ Vector2::zero() },
      LostDetectionRange{ Vector2::zero() }
  {}

  FiddlerData::FiddlerData(FiddlerData& d) :
      waypoint{ d.waypoint },
      ChargeDetectionRange{ d.ChargeDetectionRange },
      LostDetectionRange{ d.LostDetectionRange }
  {}

  SerpentipedeData::SerpentipedeData() :
      MaxCooldown{ 0.0f },
      Points{ Vector2::zero(),Vector2::zero(),Vector2::zero() },
      DetectionRange{ Vector2::zero() }
  {}

  SerpentipedeData::SerpentipedeData(SerpentipedeData& d) :
      MaxCooldown{ d.MaxCooldown },
      Points{ d.Points[0],d.Points[1],d.Points[2] },
      DetectionRange{ d.DetectionRange }
  {}

}
