#include "AI_State.h"
#include "AI_Transition.h"
#include "Core/Utils/Random.h"
#include "Core/GlobalStruct.h"

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

  IdleLancer::IdleLancer()
  {
    TransitionEdges["detect_enemy_lancer"] = new DetectEnemyLancer();
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
        if (env.pECS->GetWorld().GetEntityManager().HasComponent<Attack>(monster) && (
            AITools::Distance_X_BetweenTwoEntities(monster, player) < 2) &&
          env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(monster).CooldownTimer <= 0)
        {
          AITools::FaceEntity(monster, player);
          env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(monster).MeleeAttack = true;
        }
        AITools::MoveTowardsEntityInX(monster, player);
      }
    });
  }

  //----------------------------------------------------------------------

  IdleFiddler::IdleFiddler(Vector2 p1, Vector2 p2)
  {
    Waypoints.push_back(p1);
    Waypoints.push_back(p2);
    TransitionEdges["detect_enemy_fiddler"] = new DetectEnemyFiddler();
  }

  void IdleFiddler::onEnter(EntityID& id)
  {
  }

  void IdleFiddler::onExit(EntityID& id)
  {
  }

  void IdleFiddler::Update(EntityID& monster)
  {
    UpdateWaypoint(monster);
    CheckEdges(monster);
  }

  ChaseEnemyFiddler::ChaseEnemyFiddler()
  {
    TransitionEdges["lost_enemy_fiddler"] = new LostEnemyFiddler();
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
        AITools::MoveTowardsEntity(monster, player);
    });
  }

  //----------------------------------------------------------------------

  IdleSerpentipede::IdleSerpentipede(Vector2 p1, Vector2 p2,Vector2 p3) :
      CurrentWayPoint(0)
  {

      //JsonFile file;
      //file.StartReader("IdleSerpent.json").LoadObject(WayPoints);
      //WayPoints[0] = p1;
      //WayPoints[1] = p2;
      //WayPoints[2] = p3;
      //TransitionEdges["detect_enemy_serpentipede"] = new DetectEnemySerpentipede(WayPoints[0]);
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

  ChaseEnemySerpentipede::ChaseEnemySerpentipede(Vector2 p) :
      DetectionRange(p)
  {
      TransitionEdges["lost_enemy_serpentipede"] = new LostEnemySerpentipede(DetectionRange);
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
      //Move to any of the 3 waypoint, attack player


      //env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& player, EntityType& et)
      //    {
      //        if (et.type == EntityCategory::E_PLAYER)
      //            AITools::MoveTowardsEntity(monster, player);
      //    });
  }

}
