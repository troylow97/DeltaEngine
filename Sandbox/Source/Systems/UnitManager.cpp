#include "UnitManager.h"

#include "Core/GlobalStruct.h"

namespace DeltaEngine
{

	EntityID UnitManager::player;
	
	EntityID UnitManager::GetPlayerID()
	{
		env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& id, Player& p)
		{
			player = id;
		});

		return player;
	}
}
