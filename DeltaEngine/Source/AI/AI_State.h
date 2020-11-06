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

namespace DeltaEngine
{
	class AIState
	{
	public:
		std::unordered_map<std::string, Transition*> TransitionEdges;
		virtual void onEnter() = 0;
		virtual void onExit() = 0;
		virtual void Update() = 0;
	};

	class Idle : public AIState
	{
	public:
		std::unordered_map<std::string, Transition*> TransitionEdges;
		virtual void onEnter() override
		{

		}
		virtual void onExit() override
		{

		}
		virtual void Update() override
		{
			std::cout << "smth" << std::endl;
		}

		Idle()
		{
			//TransitionEdges["saw_enemy"] = new SawEnemy;
		}
	};

}


