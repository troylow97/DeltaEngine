#pragma once
#include "Core/Math/Vector.h"
#include "Components/Collider.h"
#include "Components/Transform.h"
#include "Components/RigidBody.h"
#include "Manifold.h"

namespace DeltaEngine
{
	bool CollisionResponse(Collider& c1, RigidBody& r1, Collider& c2, RigidBody& r2, Manifold& m);
	void ResolveContactVelocity(RigidBody& r1, RigidBody& r2, Manifold& m);
	void ResolvePenetration(RigidBody& r1, Transform& t1, RigidBody& r2, Transform& t2, Manifold& m);

}
