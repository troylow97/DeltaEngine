#pragma once
#include "Core/Math/Vector.h"

namespace DeltaEngine
{
	struct RigidBody
	{
		float Mass;
		Vector2 Direction;
		Vector2 Velocity;
		Vector2 ReflectedVector;
		float Movespeed;
		Vector2 Acceleration;
		float Restituition;
		bool hasGravity;

		RigidBody() :
			Mass{50.0f},
			Velocity{0,0},
			Acceleration{0,0},
			hasGravity{false}
		{}
	};
}