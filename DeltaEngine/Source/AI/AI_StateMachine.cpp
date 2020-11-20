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
#include "AI_StateMachine.h"

#include "Core/Debugging/Profiler/Profiler.h"
#define STATE_ENTER 0;
#define STATE_UPDATE 1;
#define STATE_EXIT 2;

namespace DeltaEngine
{
  void AISystem::Initialize()
  {
    StateList["idle_lancer"] = new IdleLancer();
    StateList["chase_enemy_lancer"] = new ChaseEnemyLancer();
    StateList["idle_fiddler"] = new IdleFiddler(Vector2{0, 0}, Vector2{5, 0});
    StateList["chase_enemy_fiddler"] = new ChaseEnemyFiddler();
    //Temporary Init
    //em.ForEach([&](EntityID id, AI ai)
    //{
    //	env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(id).key = "idle_lancer";
    //	env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(id).transition = "null";
    //	env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(id).type = "monster";
    //});


    //em.ForEach([&](EntityID id,Input i)
    //{
    //	env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(id).type = "player";
    //});
  }

  void AISystem::Shutdown()
  {
    for (auto it : StateList)
    {
      delete it.second;
    }
  }

  void AISystem::Update()
  {
    //Check and apply transitions
    em.ForEach([&](EntityID& id, AI& ai)
    {
      //bool isChanged{ false };
      AIState* ai_state = nullptr;

      auto it = StateList.find(ai.key);
      if (it != StateList.end())
      {
        ai_state = it->second;
      }
      else
      {
        return;
      }


      if (ai_state != nullptr)
      {
        if (ai.transition == "null")
        {
          ai_state->Update(id);
          return;
        }

        ai_state->onExit(id);

        ai.key = ai.transition;
        ai.transition = "null";

        auto find = StateList.find(ai.key);
        if (find != StateList.end())
        {
          ai_state = find->second;
        }
        else
        {
          return;
        }

        ai_state->onEnter(id);
      }
    });
    Profiler::Instance().Record("AI System");
  }

  void AISystem::LateUpdate()
  {
  }
}
