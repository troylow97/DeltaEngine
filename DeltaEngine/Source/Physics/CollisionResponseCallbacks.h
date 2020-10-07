#pragma once
#include "Core/Math/Vector.h"
#include "Collider.h"
#include "RigidBody.h"

namespace DeltaEngine
{
	void AABB_CollisionResponse(Collider& obj1, Collider& obj2, Vector2& obj1_vel, Vector2& obj2_vel);
	Vector2 CalculateAabbDistanceTo(Collider& e1, Collider& e2); //Helper

	void CollisionResponse_CircleCircle(Collider& col1, RigidBody& r1, Collider& col2, RigidBody& r2);

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
