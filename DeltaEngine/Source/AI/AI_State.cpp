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
    auto& ref = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(monster).position;

    if (ref.y < 2.0)
        AITools::MoveTowardsPoint(monster, Vector2{ ref.x,Random::RandomFloatRange(2.1,2.5) });
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
            AITools::Distance_X_BetweenTwoEntities(monster, player) < 2) && AITools::Distance_Y_BetweenTwoEntities(monster, player) < 1
            && env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(monster).CooldownTimer <= 0)
        {
          AITools::FaceEntity(monster, player);
          env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(monster).MeleeAttack = true;
        }
        auto player_pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(player).position;
        auto player_size = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(player).scale;
        AITools::MoveTowardsPoint(monster, Vector2{player_pos.x + Random::RandomFloatRange(-0.3,0.3),player_pos.y + Random::RandomFloatRange(0.5,1.5)});
      }
    });
  }

  //----------------------------------------------------------------------

  IdleFiddler::IdleFiddler()
  {
    //To read from json file instead
    //waypoint.Waypoints.push_back(Vector2{ 0,0 });
    //waypoint.Waypoints.push_back(Vector2{ 5,0 });


    JsonFile file;
   //rttr::variant v{ waypoint.Waypoints };
   //auto& seq = v.create_sequential_view();
    //file.StartWriter( filename ).StartObject().WriteObject( obj ).EndObject();
    file.StartReader("idle_fiddler.json").LoadObject(waypoint).EndReader();

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
    waypoint.UpdateWaypoint(monster);
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

  IdleSerpentipede::IdleSerpentipede()
  {
      StartPoint = Vector2{ 0,0 };
      JsonFile file;

      file.StartWriter("idle_serpentipede.json").StartObject().WriteObject( StartPoint ).EndObject();

      TransitionEdges["detect_enemy_serpentipede"] = new DetectEnemySerpentipede(StartPoint);
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

  ChaseEnemySerpentipede::ChaseEnemySerpentipede() :
      Points{ Vector2{0.0,0.0},Vector2{2.0,0.0},Vector2{5.0,0.0} },
      CooldownTimer{0.0f},
      CurrentPoint{0}
  {
     Points[0] = Vector2{ 0,0 };
     Points[1] = Vector2{ 2,0 };
     Points[2] = Vector2{ 5,0 };
     JsonFile file;
     file.StartWriter("chase_serpentipede.json").StartObject().WriteObject(Points).EndObject();
      TransitionEdges["lost_enemy_serpentipede"] = new LostEnemySerpentipede(Points[0]);
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
          if (AITools::EntityisAtPoint(monster, Points[CurrentPoint]))
          {
              CurrentPoint = Random::RandomIntRange(0, 2);

              env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& player, EntityType& et)
                  {
                      if (et.type == EntityCategory::E_PLAYER)
                      {
                          if (AITools::EntityisAtPoint(monster, Points[CurrentPoint]))
                          {
                              if (env.pECS->GetWorld().GetEntityManager().HasComponent<Attack>(monster) && (
                                  AITools::Distance_X_BetweenTwoEntities(monster, player) < 5) &&
                                  env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(monster).CooldownTimer <= 0)
                              {
                                  AITools::FaceEntity(monster, player);
                                  env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(monster).RangeAttack = true;
                                  CooldownTimer = 2.0f;
                              }
                          }
                      }
                  });
          }
          else
          {
              AITools::MoveTowardsPoint(monster, Points[CurrentPoint]);
          }
      }
      else
      {
          CooldownTimer -= env.pClock->DeltaTime();
      }



      //if (CooldownTimer >= 0)
      //{
      //    CooldownTimer -= env.pClock->DeltaTime();
      //
      //    //Do attack
      //    env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& player, EntityType& et)
      //    {
      //        if (et.type == EntityCategory::E_PLAYER)
      //        {
      //            if (AITools::EntityisAtPoint(monster, Points[CurrentPoint]))
      //            {
      //                if (env.pECS->GetWorld().GetEntityManager().HasComponent<Attack>(monster) && (
      //                    AITools::Distance_X_BetweenTwoEntities(monster, player) < 5) &&
      //                    env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(monster).CooldownTimer <= 0)
      //                {
      //                    AITools::FaceEntity(monster, player);
      //                    env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(monster).RangeAttack = true;
      //                    std::cout << "Ranged Attack" << std::endl;
      //                    CooldownTimer = 2.0f;
      //                    CurrentPoint = Random::RandomIntRange(0, 2);
      //                }
      //            }
      //            else
      //                AITools::MoveTowardsPoint(monster, Points[CurrentPoint]);
      //        }
      //    });
      //
      //}
      //else
      //{
      //    CooldownTimer = 2.0f;
      //    CurrentPoint = Random::RandomIntRange(0, 2);
      //}

      //Move to any of the 3 waypoint, attack player


      //env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& player, EntityType& et)
      //    {
      //        if (et.type == EntityCategory::E_PLAYER)
      //            AITools::MoveTowardsEntity(monster, player);
      //    });
  }

}
