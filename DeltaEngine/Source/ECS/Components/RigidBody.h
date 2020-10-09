#pragma once

#include "Core/Math/Vector.h"
#include "Components.h"
#include <rapidjson/prettywriter.h>
#include <rapidjson/filewritestream.h>

namespace DeltaEngine
{
	struct RigidBody
	{
		Vector2 Velocity;
		Vector2 Acceleration;
		float Mass;
		bool hasGravity;

		RigidBody() :
			Velocity{0,0},
			Acceleration{0,0},
			Mass{50.0f},
			hasGravity{false}
		{}

		REGISTER_COMPONENT(RigidBody)
	};
}