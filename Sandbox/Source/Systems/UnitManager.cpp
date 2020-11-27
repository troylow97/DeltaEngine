#include "UnitManager.h"

namespace DeltaEngine
{
	EntityID UnitManager::playerID;
	
	void UnitManager::Update()
	{
		em.ForEach([&](EntityID& id,Player& p)
		{
			playerID = id;
		});
	}

	EntityID UnitManager::GetPlayerID()
	{
		return playerID;
	}
}
