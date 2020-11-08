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
#define STATE_ENTER 0;
#define STATE_UPDATE 1;
#define STATE_EXIT 2;

namespace DeltaEngine
{
	//void AISystem::Init()
	//{
	//	StateList["idle"] = new Idle();
	//	em.ForEach([&](EntityID id, AI ai)
	//		{
	//			ai.key = "idle";
	//		});
	//}

	void AISystem::Update()
	{
		if (!isInit)
		{
			std::cout << "here0" << std::endl;
			StateList.insert({ "idle", new Idle() });
			em.ForEach([&](EntityID id, AI ai)
			{
				ai.key = "idle";
			});

			isInit = true;
		}
		//Check and apply transitions
       em.ForEach([&](EntityID id,AI ai)
       {
			ai.key = "idle";
			bool isChanged{ false };
			AIState* ai_state = nullptr;

			auto it = StateList.find(ai.key);
			if (it != StateList.end())
				ai_state = it->second;

			std::cout << ai.key << std::endl;
			std::cout << "here1" << std::endl;
			if (ai_state != nullptr)
			{
				std::cout << "here2" << std::endl;
				[&]()
				{
					for (std::pair<std::string, Transition*> ref : ai_state->TransitionEdges)
					{
						if (ref.second->isTriggered)
						{
							std::cout << "here3" << std::endl;
							ai_state->onExit(id);
							ai.key = ref.first;
							ai_state = StateList[ref.first];
							ai_state->onEnter(id);
							isChanged = true;
						}
					}

				}();

				//No transitions occured, AI updates its current state
				if (!isChanged)
				{
					std::cout << "here4" << std::endl;
					ai_state->Update(id);
				}

			}
       });
	}

	void AISystem::LateUpdate()
	{

	}
}