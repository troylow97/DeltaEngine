/**********************************************************************************
* \file   AI_Transition.h
* \brief  The file contains the code for AI Transitions
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include <string>
#include "Core/GlobalStruct.h"
#include "ECS/Entities.h"
#include "AITools.h"

namespace DeltaEngine
{
  class Transition
  {
  public:
    virtual bool TestEdge(EntityID&) = 0;
    virtual std::string getTargetState() = 0;
    virtual ~Transition() = default;
  };

  class DetectEnemyLancer : public Transition
  {
      Vector2 ChargeDetectionRange;
  public:
      DetectEnemyLancer(Vector2& charge_range) :
          ChargeDetectionRange(charge_range)
      {}

    bool TestEdge(EntityID& monster) override
    {
      auto& ref = env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(monster);
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& player, EntityType& et)
      {
        if (et.type == EntityCategory::E_PLAYER && AITools::EntityisWithinDetectionRange(monster, player, ChargeDetectionRange.x, ChargeDetectionRange.y))
        {
          ref.transition = getTargetState();
        }
      });
      if (ref.transition == getTargetState()) { return true; }
      return false;
    }

    std::string getTargetState() override
    {
      return "chase_enemy_lancer";
    }
  };

  //class LostEnemyLancer : public Transition //deprecated
  //{
  //public:
  //  bool TestEdge(EntityID& monster) override
  //  {
  //    auto& ref = env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(monster);
  //    env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& player, EntityType& et)
  //    {
  //      if (et.type == EntityCategory::E_PLAYER && !AITools::EntityisWithinDetectionRange(monster, player, 5.0f, 5.0f))
  //      {
  //        ref.transition = getTargetState();
  //      }
  //    });
  //    if (ref.transition == getTargetState()) { return true; }
  //    return false;
  //  }
  //
  //  std::string getTargetState() override
  //  {
  //    return "idle_lancer";
  //  }
  //};

  class DetectEnemyFiddler : public Transition
  {
      Vector2 ChargeDetectionRange;
  public:
    DetectEnemyFiddler(Vector2& charge_range) :
        ChargeDetectionRange(charge_range)
    {}

    bool TestEdge(EntityID& monster) override
    {
      auto& ref = env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(monster);
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& player, EntityType& et)
      {
        if (et.type == EntityCategory::E_PLAYER && AITools::EntityisWithinDetectionRange(monster, player, ChargeDetectionRange.x, ChargeDetectionRange.y))
        {
          ref.transition = getTargetState();
        }
      });
      if (ref.transition == getTargetState()) { return true; }
      return false;
    }

    std::string getTargetState() override
    {
      return "chase_enemy_fiddler";
    }
  };

  class LostEnemyFiddler : public Transition
  {
      Vector2 LostDetectionRange;
  public:
    LostEnemyFiddler(Vector2& lost_range) :
        LostDetectionRange{lost_range}
    {}

    bool TestEdge(EntityID& monster) override
    {
      auto& ref = env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(monster);
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& player, EntityType& et)
      {
        if (et.type == EntityCategory::E_PLAYER && 
            !AITools::EntityisWithinDetectionRange(monster, player, LostDetectionRange.x, LostDetectionRange.y))
        {
          ref.transition = getTargetState();
        }
      });
      if (ref.transition == getTargetState()) { return true; }
      return false;
    }

    std::string getTargetState() override
    {
      return "idle_fiddler";
    }
  };

  class DetectEnemySerpentipede : public Transition
  {
      Vector2 DetectionRange;
  public:
      DetectEnemySerpentipede(Vector2 dr) :
          DetectionRange{dr}
      {}
      bool TestEdge(EntityID& monster) override
      {
          auto& ref = env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(monster);
          env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id,Player& p)
              {
                  if (AITools::EntityisWithinDetectionRange (id, ref.original_point, 
                         DetectionRange.x, 
                         DetectionRange.y))
                  {
                      ref.transition = getTargetState();
                  }
              });
          if (ref.transition == getTargetState()) { return true; }
          return false;
      }

      std::string getTargetState() override
      {
          return "chase_enemy_serpentipede";
      }
  };

  class LostEnemySerpentipede : public Transition
  {
      Vector2 DetectionRange;
  public:
      LostEnemySerpentipede(Vector2 detection) :
          DetectionRange(detection)
      {}

      bool TestEdge(EntityID& monster) override
      {
          auto& ref = env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(monster);
          env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& player, EntityType& et)
              {
                  if (et.type == EntityCategory::E_PLAYER 
                      && !AITools::EntityisWithinDetectionRange
                          (player, ref.original_point, 
                          DetectionRange.x, 
                          DetectionRange.y))
                  {
                      ref.transition = getTargetState();
                  }
              });
          if (ref.transition == getTargetState()) { return true; }
          return false;
      }

      std::string getTargetState() override
      {
          return "idle_serpentipede";
      }
  };
}
