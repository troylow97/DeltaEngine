#pragma once
#include "Core/Math/Vector.h"

namespace DeltaEngine
{
	struct RigidBody
	{
		bool isMoveable;
		float Mass;
		Vector2 Direction;
		Vector2 Velocity;
		Vector2 ReflectedVector;
		float Friction;
		float Movespeed;
		float inherentAcceleration;
		Vector2 Acceleration;
		float Restituition; //not used
		bool hasGravity;

		RigidBody() :
			Mass{50.0f},
			Velocity{0,0},
			Acceleration{0,0},
			hasGravity{false},
			Friction{0.97},
			inherentAcceleration{3.0f},
			isMoveable{true}
		{}
	};
}