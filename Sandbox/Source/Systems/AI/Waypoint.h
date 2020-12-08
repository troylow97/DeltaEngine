/**********************************************************************************
* \file   Waypoint.h
* \brief  The file contains the code for waypoint logic, to be used in AI
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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

