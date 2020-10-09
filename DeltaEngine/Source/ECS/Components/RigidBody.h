#pragma once

#include "Core/Math/Vector.h"
#include "Components.h"
#include <rapidjson/prettywriter.h>
#include <rapidjson/filewritestream.h>

namespace DeltaEngine
{
	struct RigidBody
	{
		bool isMoveable;
		float Mass;
		Vector2 Direction;
		Vector2 Velocity;
		Vector2 Acceleration;
		float Mass;
		bool hasGravity;

		RigidBody() :
			Velocity{0,0},
			Acceleration{0,0},
			hasGravity{false},
			Friction{0.97},
			inherentAcceleration{3.0f},
			isMoveable{true}
		{}

		REGISTER_COMPONENT(RigidBody)
	};
}