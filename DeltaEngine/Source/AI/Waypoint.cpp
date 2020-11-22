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

		if(AITools::EntityisAtPoint(id, Waypoints[CurrentWaypoint]))
		{
			if (CurrentWaypoint < Waypoints.size() - 1)
				CurrentWaypoint++;
			else
				CurrentWaypoint = 0;
		}

		AITools::MoveTowardsPoint(id, Waypoints[CurrentWaypoint]);
	}
}