/**********************************************************************************
* \file   Waypoint.cpp
* \brief  The file contains BLAHBLAHBLAH
* \author Chin, Clara,   X% Code Contribution
* \author Low, Troy,     X% Code Contribution
* \author Ong, Graeme,   X% Code Contribution
* \author Tan, Tong Wee, X% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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
		if(AITools::EntityisAtPointInX(id, original_pos.x + Waypoints[CurrentWaypoint].x,0.1f))
		{
			if (CurrentWaypoint < Waypoints.size() - 1)
				CurrentWaypoint++;
			else
				CurrentWaypoint = 0;
		}

		AITools::MoveTowardsPoint(id, original_pos + Waypoints[CurrentWaypoint]);
	}
}