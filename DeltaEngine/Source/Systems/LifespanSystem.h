#pragma once
#include "ECS/ECSModule.h"
#include "Components/Lifespan.h"
#include <vector>

namespace DeltaEngine
{
	DEFINE_SYSTEM(LifespanSystem, Lifespan)
		void Update() override;
		void LateUpdate() override;
private:
	std::vector<EntityID> DestroyedEntities;
	END_DEFINE_SYSTEM(LifespanSystem)
}
