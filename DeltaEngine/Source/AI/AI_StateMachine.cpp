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
#include "LancerAI.h"

namespace DeltaEngine
{
	void AISystem::Initialize()
	{
		AISystem::StateList["idle_lancer"] = new IdleLancer();
		AISystem::StateList["chase_enemy_lancer"] = new ChaseEnemyLancer();
		//AISystem::StateList["idle_fiddler"] = new IdleFiddler(Vector2{ 0,0 }, Vector2{ 5,0 });
		//AISystem::StateList["chase_enemy_fiddler"] = new ChaseEnemyFiddler();
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
       em.ForEach([&](EntityID& id,AI& ai)
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
	}

	void AISystem::LateUpdate()
	{

	}
}