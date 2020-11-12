#pragma once
#include "ECS/ECSModule.h"
#include "Components/Lifespan.h"

namespace DeltaEngine
{
	DEFINE_SYSTEM(LifespanSystem, Lifespan)
		void Update() override;
		void LateUpdate() override;

	END_DEFINE_SYSTEM(LifespanSystem)
}
