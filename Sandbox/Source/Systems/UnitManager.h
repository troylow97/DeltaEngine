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
