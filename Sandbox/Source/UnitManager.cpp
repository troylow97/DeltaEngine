#include "UnitManager.h"
#include "ECS/ECSModule.h"
#include "Core/GlobalStruct.h"

UnitManager& GetUnitManager()
{
	return unit_manager;
}

void UnitManager::Initialize()
{
	DeltaEngine::env.pECS->GetWorld().GetEntityManager().ForEach([&](DeltaEngine::EntityID& id,DeltaEngine::EntityType et)
	{
		if (et.type == DeltaEngine::EntityCategory::E_PLAYER)
		{
			player = id;
		}
	});
}