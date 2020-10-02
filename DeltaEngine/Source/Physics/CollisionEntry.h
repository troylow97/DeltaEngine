#pragma once
#include "Collider.h"

namespace DeltaEngine
{
	/*
	For each collision detected, a new CollisionEntry object is created.
	This CollisionEntry stores all the information about the collision,
	including the two objects (nodes) involved in the collision,
	and the point of impact and the surface normal of the into object at that point.
	*/
	class DE_API ColliderPair
	{
		Collider* collider1;
		Collider* collider2;
		
	};
}
