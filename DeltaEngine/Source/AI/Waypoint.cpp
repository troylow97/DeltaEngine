#include "Waypoint.h"
#include "Components/Transform.h"
#include "Core/GlobalStruct.h"
#include "AITools.h"

namespace DeltaEngine
{
	Waypoint::Waypoint() :
		CurrentWaypoint(0)
	{}

	void Waypoint::UpdateWaypoint(EntityID id)
	{
		auto pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id).position;
		auto original_pos = env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(id).original_point;
		if(AITools::EntityisAtPointInX(id, original_pos.x + Waypoints[CurrentWaypoint].x))
		{
			if (CurrentWaypoint < Waypoints.size() - 1)
				CurrentWaypoint++;
			else
				CurrentWaypoint = 0;
		}

		AITools::MoveTowardsPoint(id, original_pos + Waypoints[CurrentWaypoint]);
	}
}