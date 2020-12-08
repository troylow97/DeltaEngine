/**********************************************************************************
* \file   UnitManager.cpp
* \brief  The file contains BLAHBLAHBLAH
* \author Chin, Clara,   X% Code Contribution
* \author Low, Troy,     X% Code Contribution
* \author Ong, Graeme,   X% Code Contribution
* \author Tan, Tong Wee, X% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "UnitManager.h"

#include "Core/GlobalStruct.h"

namespace DeltaEngine
{

	EntityID UnitManager::player;
	
	EntityID UnitManager::GetPlayerID()
	{
		env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& id, Player& p, RigidBody& r, State& s, Animator& a)
		{
			player = id;
		});
		return player;
	}
}
