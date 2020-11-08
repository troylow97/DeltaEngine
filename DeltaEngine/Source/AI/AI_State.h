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
#include <unordered_map>
#include "AI_Transition.h"
#include "Core/GlobalStruct.h"
namespace DeltaEngine
{
	class AIState
	{
	public:
		std::unordered_map<std::string, Transition*> TransitionEdges;
		virtual void onEnter(EntityID& id) = 0;
		virtual void onExit(EntityID& id) = 0;
		virtual void Update(EntityID& id) = 0;
	};

	class Idle : public AIState
	{
	public:
		std::unordered_map<std::string, Transition*> TransitionEdges;
		virtual void onEnter(EntityID& id) override
		{

		}
		virtual void onExit(EntityID& id) override
		{

		}
		virtual void Update(EntityID& id) override
		{
			env.pECS->GetWorld().get_entity_manager().ForEach([&](EntityID id1, AI ai)
			{
				if (id.index != id1.index)
				{

				}
			});

		}
	};

}


