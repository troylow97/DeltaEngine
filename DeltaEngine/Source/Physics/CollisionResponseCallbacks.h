#pragma once
#include "Core/Math/Vector.h"
#include "Collider.h"
#include "RigidBody.h"
#include "Core/Math/Transform.h"
#include "Manifold.h"
namespace DeltaEngine
{
	void CollisionResponse_AABBvsAABB(Collider& obj1, RigidBody& r1, Collider& obj2, RigidBody& r2);
	void CollisionResponse_CirclevsCircle(Collider& col1, RigidBody& r1, Transform& t1, Collider& col2, RigidBody& r2, Transform& t2, Manifold& m);
	void CollisionResponse_RectvsCircle(Collider& col1, RigidBody& r1, Transform& t1, Collider& col2, RigidBody& r2, Transform& t2, Manifold& m);

	void CollisionResponse_Main(Collider& col1, RigidBody& r1, Transform& t1, Collider& col2, RigidBody& r2, Transform& t2, Manifold& m);
	void CollisionResponse_Box(Collider& col1, RigidBody& r1, Transform& t1, Collider& col2, RigidBody& r2, Transform& t2, Manifold& m);
	void CollisionResponse_Circle(Collider& col1, RigidBody& r1, Transform& t1, Collider& col2, RigidBody& r2, Transform& t2, Manifold& m);
	/******************************************************************************/
	/*!
		Calculates reflection when the circle hits a pillar.
		A reflection vector is calculated and the end point is also updated.
	 */
	 /******************************************************************************/
	//void CollisionResponse_CirclePillar(Vector2& normal,
	//	const float& interTime,
	//	const Vector2& ptStart,
	//	const Vector2& ptInter,
	//	Vector2& ptEnd,
	//	Vector2& reflectedVectorNormalized)
	//{
	//	//penetration is how much excess your velocity will go through
	//	Vector2 penetration = ptStart - ptInter;
	//
	//	//calculate the reflection vector based on direction of ball and normal
	//	reflectedVectorNormalized = (((normal * (penetration.DotProduct(normal))) * 2) - penetration).Normalize();
	//
	//	//update ptEnd which is the next position of the ball
	//	float v = (ptEnd - ptStart).Magnitude();
	//	ptEnd = ptInter + reflectedVectorNormalized * (1 - interTime) * v;
	//}

}
