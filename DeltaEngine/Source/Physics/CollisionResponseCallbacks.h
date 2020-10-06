#pragma once
#include "Core/Math/Vector.h"
#include "Collider.h"

namespace DeltaEngine
{
	void AABB_CollisionResponse(Collider& obj1, Collider& obj2, Vector2& obj1_vel, Vector2& obj2_vel);
	Vector2 CalculateDistanceTo(Collider& e1, Collider& e2); //Helper function
	/******************************************************************************/
	/*!
		Calculates reflection when the circle hits another circle.
		A reflection vector is calculated and the end point is also updated.

		This calculation takes into account the mass of two circles.
	 */
	 /******************************************************************************/
	//void CollisionResponse_CircleCircle(Vector2& normal,
	//	const float interTime,
	//	Vector2& velA,
	//	const float& massA,
	//	Vector2& interPtA,
	//	Vector2& velB,
	//	const float& massB,
	//	Vector2& interPtB,
	//	Vector2& reflectedVectorA,
	//	Vector2& ptEndA,
	//	Vector2& reflectedVectorB,
	//	Vector2& ptEndB)
	//{
	//	//get the direction of reflection using dot product
	//	float aA = velA.DotProduct(normal);
	//	float aB = velB.DotProduct(normal);
	//
	//	//calculate reflection vector based on conservation of momentum and direction based on the normal and velocity
	//	reflectedVectorA = velA - normal * massB * (2 * (aA - aB) / (massA + massB));
	//	reflectedVectorB = velB + normal * massA * (2 * (aA - aB) / (massA + massB));
	//
	//	//update the end points of where the two circles will end up
	//	ptEndA = interPtA + reflectedVectorA * (1.0f - interTime);
	//	ptEndB = interPtB + reflectedVectorB * (1.0f - interTime);
	//}

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
