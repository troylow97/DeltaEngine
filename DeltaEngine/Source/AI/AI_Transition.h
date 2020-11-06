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
//#include "Core/Containers/ActionList.h"
#include "Core/TypeAlias.h"

namespace DeltaEngine
{	class Transition
	{
	public:
		bool isTriggered;
		virtual std::string getTargetState() = 0;

	};

	class SawEnemy : public Transition
	{
	public:
		bool isTriggered;
		virtual std::string getTargetState()
		{
			return "idle";
		}
	};
}