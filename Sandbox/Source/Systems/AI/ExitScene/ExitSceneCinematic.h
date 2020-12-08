#pragma once
#include "Core/Math/Vector.h"
#include "ECS/ECSModule.h"
#include "Components/Transform.h"
namespace DeltaEngine
{
	DEFINE_SYSTEM(ExitSceneCinematic, Transform)
    void Initialize() override;
	void Update() override;
	void LateUpdate() override;

private:
	Vector2 ExitPoint;
	Vector2 StopPoint;
	bool ExitPointTriggered;
	bool StopPointTriggered;

	END_DEFINE_SYSTEM(ExitSceneCinematic)

}
