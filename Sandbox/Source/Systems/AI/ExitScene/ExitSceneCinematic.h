#pragma once
#include "Core/Math/Vector.h"
#include "ECS/ECSModule.h"
#include "Components/Components.h"

namespace DeltaEngine
{
	DEFINE_SYSTEM(ExitSceneCinematic, Transform, EntityType)
    void Initialize() override;
	void Update() override;

private:
	Vector2 ExitPoint;
	Vector2 StopPoint;
	bool ExitPointTriggered;
	bool StopPointTriggered;

	END_DEFINE_SYSTEM(ExitSceneCinematic)

}
