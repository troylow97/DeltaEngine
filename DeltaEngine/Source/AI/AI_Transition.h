/**********************************************************************************
* \file			AIStates
* \brief		Contains all the base abstract class of any AI State
* \author		Low Yee Troy, 100% Code Contribution
* \version		1.0
* \date			2020
*
* \note			Course: GAM200
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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
  public:
    bool TestEdge(EntityID& monster) override
    {
      auto& ref = env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(monster);
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& player, EntityType& et)
      {
        if (et.type == EntityCategory::E_PLAYER && AITools::EntityisWithinDetectionRange(monster, player, 3.0f, 5.0f))
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

  class LostEnemyLancer : public Transition
  {
  public:
    bool TestEdge(EntityID& monster) override
    {
      auto& ref = env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(monster);
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& player, EntityType& et)
      {
        if (et.type == EntityCategory::E_PLAYER && !AITools::EntityisWithinDetectionRange(monster, player, 5.0f, 5.0f))
        {
          ref.transition = getTargetState();
        }
      });
      if (ref.transition == getTargetState()) { return true; }
      return false;
    }

    std::string getTargetState() override
    {
      return "idle_lancer";
    }
  };

  class DetectEnemyFiddler : public Transition
  {
  public:
    bool TestEdge(EntityID& monster) override
    {
      auto& ref = env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(monster);
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& player, EntityType& et)
      {
        if (et.type == EntityCategory::E_PLAYER && AITools::EntityisWithinDetectionRange(monster, player, 3.0f, 5.0f))
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
  public:
    bool TestEdge(EntityID& monster) override
    {
      auto& ref = env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(monster);
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& player, EntityType& et)
      {
        if (et.type == EntityCategory::E_PLAYER && !AITools::EntityisWithinDetectionRange(monster, player, 5.0f, 5.0f))
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
}
