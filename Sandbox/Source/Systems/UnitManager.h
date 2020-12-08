/**********************************************************************************
* \file   UnitManager.h
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
#pragma once
#include "ECS/ECSModule.h"

namespace DeltaEngine
{
class UnitManager
{
	static EntityID player;

public:
	static EntityID GetPlayerID();

};
	
//DEFINE_SYSTEM(UnitManager, Player)
//  void UnitManager::Update() override;
//  static EntityID GetPlayerID();
//private:
//  static EntityID playerID;
//	
//END_DEFINE_SYSTEM(UnitManager)
}
