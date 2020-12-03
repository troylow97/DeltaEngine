#pragma once
#include "Core/Math/Vector.h"
#include "ECS/ECSModule.h"
#include <vector>

namespace DeltaEngine
{
	struct Waypoint
	{
		std::vector<Vector2> Waypoints;
		unsigned CurrentWaypoint;
		Waypoint();
		void UpdateWaypoint(EntityID id);
	};
}

