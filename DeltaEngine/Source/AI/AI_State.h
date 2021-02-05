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

namespace DeltaEngine
{
	class Transition;

	class AIState
	{
	protected:
		bool CheckEdges(EntityID&);
	public:
		std::unordered_map<std::string, Transition*> TransitionEdges;
		virtual void onEnter(EntityID& id) = 0;
		virtual void onExit(EntityID& id) = 0;
		virtual void Update(EntityID& id) = 0;
		virtual ~AIState();
	};
}


