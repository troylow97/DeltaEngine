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
    SerpentipedeAIData serpent_data;
    JsonFile file;
    file.StartReader("AI/serpentipedeAI.json").LoadObject(serpent_data).EndReader();

    FiddlerAIData fiddler_data;
    JsonFile file2;
    //file2.StartWriter("AI/fiddler.json").StartObject().WriteObject(data2).EndObject().EndWriter();
    file2.StartReader("AI/fiddlerAI.json").LoadObject(fiddler_data).EndReader();
    file2.StartReader("AI/fiddlerAI.json").LoadObject(fiddler_data.waypoint).EndReader();

    LancerAIData lancer_data;
    JsonFile file3;
    file3.StartReader("AI/lancerAI.json").LoadObject(lancer_data).EndReader();

    StateList["idle_lancer"] = new IdleLancer(lancer_data.ChargeDetectionRange);
    StateList["chase_enemy_lancer"] = new ChaseEnemyLancer();

    StateList["idle_fiddler"] = new IdleFiddler(fiddler_data.waypoint, fiddler_data.ChargeDetectionRange);
    StateList["chase_enemy_fiddler"] = new ChaseEnemyFiddler(fiddler_data.LostDetectionRange);

    StateList["idle_serpentipede"] = new IdleSerpentipede(serpent_data.DetectionRange);
    StateList["chase_enemy_serpentipede"] = new ChaseEnemySerpentipede(serpent_data);
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
