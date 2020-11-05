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


namespace DeltaEngine
{
	namespace AI
	{
		void AISystem::Update()
		{
            em.ForEach([&](EntityID id)
            {
                

            });
		}

		void AISystem::LateUpdate()
		{

		}
	}
}