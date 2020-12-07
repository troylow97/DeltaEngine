#pragma once
#include "Core/Math/Vector.h"
#include "ECS/ECSModule.h"

namespace DeltaEngine
{
	DEFINE_SYSTEM(ExitScene, Transform, EntityType)
    void Initialize() override;
	void Update() override;

private:
	Vector2 ExitPoint;

	END_DEFINE_SYSTEM(ExitScene)


}
