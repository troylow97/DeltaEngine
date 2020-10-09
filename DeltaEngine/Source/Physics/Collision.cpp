/******************************************************************************/
/*!
\file		Collision.cpp
\author 	Low Yee Troy
\par    	email: troy.low\@digipen.edu
\date   	March 10, 2020
\brief		This file contains the function for AABB collision detection.

Copyright (C) 2017 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "Physics/Collision.h"
#include "Core/Math/Math.h"
#include "Core/GameClock/GameClock.h"
#include "Core/GlobalStruct.h"

namespace DeltaEngine
{
	/******************************************************************************/
	/*!
		Builds a line segment when given a position, scale and direction.
		The position given would be the midpoint of the line segment and it
		would extend in two opposite directions
	 */
	 /******************************************************************************/
	void BuildLineSegment(LineSegment& lineSegment,
		const  Vector2& pos,
		float scale,
		float dir)
	{
		//Creates a vector based on the given direction and scale
		Vector2 v;
		v.x = cosf(dir) * scale;
		v.y = sinf(dir) * scale;

		//Halfs the vector so that it can be added to the midpoint called pos
		v.x /= 2;
		v.y /= 2;

		//Initialises the position of the two end points of the line segment
		lineSegment.m_pt0 = pos + v;
		lineSegment.m_pt1 = pos - v;

		//Calculate the normal of the line segment based on the vector and normalise it
		lineSegment.m_normal.x = v.y;
		lineSegment.m_normal.y = -v.x;

		lineSegment.m_normal = lineSegment.m_normal.Normalize();
	}

	bool CollisionIntersection_RectRect_Static(const Vector2 Center1, const Vector2 Size1, const Vector2 Center2, const Vector2 Size2)
	{
		AABB aabb1{ Center1,Size1 };
		AABB aabb2{ Center2, Size2 };

		if (!((aabb1.max.x < aabb2.min.x) || (aabb1.min.x > aabb2.max.x) || (aabb1.max.y < aabb2.min.y) || (aabb1.min.y > aabb2.max.y)))
			return true;

		return false;
	}

	//need to test
	bool CollisionIntersection_RectRay_Static(const Vector2 Center1,const Vector2 Size1,const Vector2 Center2,const Vector2 Size2)
	{
		AABB aabb{ Center1,Size1 };
		Ray r{ Center2,Size2 };
		float tx1 = (aabb.min.x - r.m_pt0.x) * r.m_dir.x;
		float tx2 = (aabb.max.x - r.m_pt0.x) * r.m_dir.x;

		float tmin = Math::math_max(tx1, tx2);
		float tmax = Math::math_min(tx1, tx2);

		float ty1 = (aabb.min.y - r.m_pt0.y) * r.m_dir.y;
		float ty2 = (aabb.max.y - r.m_pt0.y) * r.m_dir.y;

		tmin = Math::math_max(tmin, Math::math_min(ty1, ty2));
		tmax = Math::math_min(tmax, Math::math_max(ty1, ty2));

		return tmax >= tmin;
	}

	bool CollisionIntersection_RectCircle_Static(const Vector2 Center1,const Vector2 Size1,const Vector2 Center2,const Vector2 Size2,Manifold& m)
	{
		Vector2 dist(Center1 - Center2);
		Vector2 CircleTip;
		CircleTip = Normalise(dist);
		CircleTip *= Size2.x;
		CircleTip += Center2;
		//m.normal = 
		if (CollisionIntersection_RectPoint(Center1, Size1, CircleTip))
			return true;
	}

	bool CollisionIntersection_RectLine_Static(const Vector2 Center1, const Vector2 Size1, const Vector2 Center2, const Vector2 Size2)
	{
		AABB aabb{ Center1,Size1 };
		LineSegment line{ Center2,Size2 };

		if (((line.m_pt0.x > aabb.min.x&& line.m_pt0.x < aabb.max.x) || (line.m_pt1.x > aabb.min.x&& line.m_pt1.x < aabb.max.x)) &&
			((line.m_pt0.y > aabb.min.y&& line.m_pt0.y < aabb.max.y) || (line.m_pt1.y > aabb.min.y&& line.m_pt1.y < aabb.max.y)))
		{
			return true;
		}

		return false;
	}

	bool CollisionIntersection_CircleLineSegment_Static(const Vector2 Center1, const Vector2 Size1, const Vector2 Center2, const Vector2 Size2)
	{
		Circle circle{ Center1,Size1 };
		LineSegment line{ Center2,Size2 };

		if ((line.m_pt0.x - circle.m_center.x) * (line.m_pt0.x - circle.m_center.x) + (line.m_pt0.y - circle.m_center.y) * (line.m_pt0.y - circle.m_center.y) <=
			(line.m_pt1.x - line.m_pt0.x) * (line.m_pt1.x - line.m_pt0.x) + (line.m_pt1.y - line.m_pt0.y) + (line.m_pt1.y - line.m_pt0.y))
		{
			return true;
		}
		if ((line.m_pt1.x - circle.m_center.x) * (line.m_pt1.x - circle.m_center.x) + (line.m_pt1.y - circle.m_center.y) * (line.m_pt1.y - circle.m_center.y) <=
			(line.m_pt1.x - line.m_pt0.x) * (line.m_pt1.x - line.m_pt0.x) + (line.m_pt1.y - line.m_pt0.y) + (line.m_pt1.y - line.m_pt0.y))
		{
			return true;
		}

		return false;
	}

	bool CollisionIntersection_CircleLineSegment_Static(const Vector2 Center1, const Vector2 Size1, LineSegment line)
	{
		Circle circle{ Center1,Size1 };

		if ((line.m_pt0.x - circle.m_center.x) * (line.m_pt0.x - circle.m_center.x) + (line.m_pt0.y - circle.m_center.y) * (line.m_pt0.y - circle.m_center.y) <=
			(line.m_pt1.x - line.m_pt0.x) * (line.m_pt1.x - line.m_pt0.x) + (line.m_pt1.y - line.m_pt0.y) + (line.m_pt1.y - line.m_pt0.y))
		{
			return true;
		}
		if ((line.m_pt1.x - circle.m_center.x) * (line.m_pt1.x - circle.m_center.x) + (line.m_pt1.y - circle.m_center.y) * (line.m_pt1.y - circle.m_center.y) <=
			(line.m_pt1.x - line.m_pt0.x) * (line.m_pt1.x - line.m_pt0.x) + (line.m_pt1.y - line.m_pt0.y) + (line.m_pt1.y - line.m_pt0.y))
		{
			return true;
		}

		return false;
	}

	/******************************************************************************/
	/*!
		Tests for collision between a circle to another circle
		This is done through ray intersection.
		One ray uses circle's A center as a point and gets the relative velocity
		by making circleB non-moving.

		A new circle is made with the combined radius of both circles
	 */
	 /******************************************************************************/
	bool CollisionIntersection_CircleCircle(Collider& col1, const Vector2& v1,Collider& col2, const Vector2& v2,Manifold& m)
	{
		//Get relative velocity by considering circleB as non-moving by taking velA - velB
		//Use the circle as a point for the ray
		Ray ray{col1.center, v1 - v2};
		
		//Use a circles center with a combined radius of both circles
		Collider col3{ col2.center,col2.size + col1.size, ColliderType::CIRCLE };

		if(CollisionIntersection_RayCircle(ray, col3,m))
		{
			//intersection when both circle collides
			col1.interPoint = col1.center + v1 * m.interTime;
			col2.interPoint = col2.center + v2 * m.interTime;

			return true;
		}
		return false;
	}

	bool CollisionIntersection_CircleCircle_Static(const Vector2 Center1, const Vector2 Size1, const Vector2 Center2, const Vector2 Size2)
	{
		Circle circle1{ Center1,Size1 };
		Circle circle2{ Center2,Size2 };

		if
			(
			(circle2.m_center.x - circle1.m_center.x) * (circle2.m_center.x - circle1.m_center.x) +
				(circle2.m_center.y - circle1.m_center.y) * (circle2.m_center.y - circle1.m_center.y) <=
				(circle1.m_radius + circle2.m_radius) * (circle1.m_radius + circle2.m_radius)
				)
		{
			return true;
		}
		
		return false;
	}

	bool CollisionIntersection_RayLine_Static(const Vector2 Center1, const Vector2 Size1, const Vector2 Center2, const Vector2 Size2)
	{
		Ray ray{ Center1,Size1 };
		LineSegment line{ Center2,Size2 };
		Vector2 v1 = ray.m_pt0 - line.m_pt0;
		Vector2 v2 = line.m_pt1 - line.m_pt0;
		Vector2 v3 = Vector2(-ray.m_dir.y, ray.m_dir.x);

		float dot = v2.DotProduct(v3);
		if (abs(dot) < 0.000001)
			return false;

		float t1 = v2.CrossProduct_Magnitude(v1) / dot;
		float t2 = v1.DotProduct(v3) / dot;

		if (t1 >= 0.0 && (t2 >= 0.0 && t2 <= 1.0))
			return t1;

		return false;
	}

	bool CollisionIntersection_LineLine_Static(const Vector2 Center1, const Vector2 Size1, const Vector2 Center2, const Vector2 Size2)
	{
		LineSegment line{ Center1,Size1 };
		LineSegment line2{ Center2,Size2 };
		float denominator = ((line.m_pt1.x - line.m_pt0.x) * (line2.m_pt1.y - line2.m_pt0.y)) - ((line.m_pt1.y - line.m_pt0.y) * (line2.m_pt1.x - line2.m_pt0.x));
		float numerator1 = ((line.m_pt0.y - line2.m_pt0.y) * (line2.m_pt1.x - line2.m_pt0.x)) - ((line.m_pt0.x - line2.m_pt0.x) * (line2.m_pt1.y - line2.m_pt0.y));
		float numerator2 = ((line.m_pt0.y - line2.m_pt0.y) * (line.m_pt1.x - line.m_pt0.x)) - ((line.m_pt0.x - line2.m_pt0.x) * (line.m_pt1.y - line.m_pt0.y));

		// Detect coincident lines (has a problem)
		if (denominator == 0) return numerator1 == 0 && numerator2 == 0;

		float r = numerator1 / denominator;
		float s = numerator2 / denominator;

		return (r >= 0 && r <= 1) && (s >= 0 && s <= 1);
	}

	bool CollisionIntersection_RectPoint(const Vector2 Center1, const Vector2 Size1, const Vector2 Center2)
	{
		Vector2 TopLeft, TopRight, BotLeft, BotRight;
		float HalfWidth = Size1.x / 2;
		float HalfHeight = Size1.y / 2;
		TopLeft.x = Center1.x - HalfWidth;
		TopLeft.y = Center1.y + HalfHeight;
		TopRight.x = Center1.x + HalfWidth;
		TopRight.y = Center1.y + HalfHeight;
		BotLeft.x = Center1.x - HalfWidth;
		BotLeft.y = Center1.y - HalfHeight;
		BotRight.x = Center1.x + HalfWidth;
		BotRight.y = Center1.y - HalfWidth;

		if (Center2.x < TopLeft.x) return false;
		if (Center2.x > TopRight.x) return false;
		if (Center2.y < BotLeft.y) return false;
		if (Center2.y > TopLeft.y) return false;
		return true;
	}

	bool CollisionIntersection_CirclePoint(const Vector2 Center1, const Vector2 Size1, const Vector2 Center2)
	{
		if ((Center2.x - Center1.x) * (Center2.x - Center1.x) + (Center2.y - Center1.y) * (Center2.y - Center1.y) <= (Size1.x * Size1.x))
		{
			return true;
		}

		return false;


	}

//DYNAMIC COLLISION CHECKS
	bool CollisionIntersection_RectRect(const Collider& col1, const Vector2& vel1, const Collider& col2, const Vector2& vel2)
	{
		AABB aabb1{col1.center,col1.size};
		AABB aabb2{col2.center,col2.size};
		//Static Collision Check
		if (!((aabb1.max.x < aabb2.min.x) || (aabb1.min.x > aabb2.max.x) || (aabb1.max.y < aabb2.min.y) || (aabb1.min.y > aabb2.max.y)))
			return true;
	
		float tFirst = 0;
		float tLast = env.pClock->DeltaTime();
		Vector2 RelativeVel;
		// getting relative velocity
		RelativeVel.x = vel2.x - vel1.x;
		RelativeVel.y = vel2.y - vel1.y;
	
		if (RelativeVel.x == 0.0f && RelativeVel.y == 0.0f)
			return false;
	
		// checking for case 1
		if (RelativeVel.x < 0.0f)
		{
			// checking for case 4
			if (aabb1.min.x > aabb2.max.x) // check A min < B max
				return false;
			if (aabb1.max.x < aabb2.min.x) // check A max < B min
				tFirst = Math::math_max((aabb1.max.x - aabb2.min.x) / RelativeVel.x, tFirst);
			if (aabb1.min.x < aabb2.max.x) // check A min < B Max
				tLast = Math::math_min((aabb1.min.x - aabb2.max.x) / RelativeVel.x, tLast);
		}
		else if (RelativeVel.x > 0.0f)  //caculate the x axis of the AABB when RelativeVel is < 0
		{
			// checking for case 2
			if (aabb1.max.x < aabb2.min.x) //check A max < B min
				return false;
			if (aabb1.min.x > aabb2.max.x) //check A min > B max
				tFirst = Math::math_max((aabb1.min.x - aabb2.max.x) / RelativeVel.x, tFirst);
			if (aabb1.max.x > aabb2.min.x) //check A min > B min
				tLast = Math::math_min((aabb1.max.x - aabb2.min.x) / RelativeVel.x, tLast);
		}
		// checking for case 3
		else if ((aabb1.max.x < aabb2.min.x) || (aabb1.min.x > aabb2.max.x))
		{
			return false;
		}
	
	
		// checking for case 1
		if (RelativeVel.y < 0.0f)
		{
			// checking for case 4
			if (aabb1.min.y > aabb2.max.y) // check A min < B max
				return false;
			if (aabb1.max.y < aabb2.min.y) // check A max < B min
				tFirst = Math::math_max((aabb1.max.y - aabb2.min.y) / RelativeVel.y, tFirst);
			if (aabb1.min.y < aabb2.max.y) // check A min < B Max
				tLast = Math::math_min((aabb1.min.y - aabb2.max.y) / RelativeVel.y, tLast);
		}
		else if (RelativeVel.y > 0.0f)
		{
			// checking for case 2
			if (aabb1.max.y < aabb2.min.y) //check A max < B min
				return false;
			if (aabb1.min.y > aabb2.max.y) //check A min > B max
				tFirst = Math::math_max((aabb1.min.y - aabb2.max.y) / RelativeVel.y, tFirst);
			if (aabb1.max.y > aabb2.min.y) //check A min > B min
				tLast = Math::math_min((aabb1.max.y - aabb2.min.y) / RelativeVel.y, tLast);
		}
		// checking for case 3
		else if ((aabb1.max.y < aabb2.min.y) || (aabb1.min.y > aabb2.max.y))
		{
			return false;
		}
	
		// checking for case 5
		if (tFirst > tLast)
			return false;
		return true;
	}

	bool CollisionIntersection_RayCircle(const Ray& ray, Collider& col2,Manifold& manifold)
	{
		Circle circle{ col2.center,col2.size };
		//Calculate end point of ray
		Vector2 Be = ray.m_pt0 + ray.m_dir;

		//ray.m_dir is the velocity, use it to get its normalised version
		Vector2 temp = ray.m_dir;
		Vector2 normalisedVelocity = Normalise(temp);
		float t0, t1;
		float radius_sqr = circle.m_radius * circle.m_radius;
		// geometric solution
		Vector2 L = circle.m_center - ray.m_pt0;
		float tca = Vector2DotProduct(L, ray.m_dir);
		if (tca < 0) return false;
		float d2 = Vector2DotProduct(L,L) - tca * tca;
		if (d2 > (radius_sqr)) return false;
		float thc = sqrt(radius_sqr - d2);
		t0 = tca - thc;
		t1 = tca + thc;

		if (t0 > t1) std::swap(t0, t1);

		if (t0 < 0) {
			t0 = t1; // if t0 is negative, let's use t1 instead 
			if (t0 < 0) return false; // both t0 and t1 are negative 
		}

		t1 = t0;

		return true;

		//Calculate end point of ray
		//Vector2 Be = ray.m_pt0 + ray.m_dir;

		//ray.m_dir is the velocity, use it to get its normalised version
	//	Vector2 temp = ray.m_dir;
	//	Vector2 normalisedVelocity = Normalise(temp);
	//
	//	Vector2 BsC = circle.m_center - ray.m_pt0;
	//
	//	// check if circle is behind ray origin
	//	float m = Vector2DotProduct(BsC, normalisedVelocity);
	//	if ((m < 0) && (Vector2Length(BsC) >= circle.m_radius))
	//	{
	//		return false;
	//	}
	//
	//	// check if ray within circle radius range
	//	float n2 = Vector2Length(BsC)- (m * m);
	//	if (n2 <= (circle.m_radius * circle.m_radius))
	//	{
	//		float s2 = sqrtf((circle.m_radius * circle.m_radius) - n2);
	//		float rayLength = Vector2Length(temp);
	//		float ti0 = (m - s2) / rayLength;
	//		float ti1 = (m + s2) / rayLength;
	//
	//		if (ti0 > ti1)
	//		{
	//			manifold.interTime = ti1;
	//		}
	//		else
	//		{
	//			manifold.interTime = ti0;
	//		}
	//
	//		if (manifold.interTime > 0 && manifold.interTime < 1)
	//			return true;
	//	}
	//	return false;
	}
//------------

	bool CollisionIntersection_Main(Collider& col1,const RigidBody& r1,Collider& col2,const RigidBody& r2,Manifold& m)
	{
		ColliderType type1 = col1.type;

		switch (type1)
		{
		case ColliderType::BOX:
			return CollisionIntersection_Sub_Box(col1, r1, col2, r2, m);
		case ColliderType::CIRCLE:
			return CollisionIntersection_Sub_Circle(col1,r1, col2,r2,m);
		case ColliderType::LINE:
			return CollisionIntersection_Sub_Line(col1, col2, m);
		case ColliderType::RAY:
			return CollisionIntersection_Sub_Ray(col1, r1, col2, r2, m);
		default:
			return CollisionIntersection_Sub_Box(col1, r1, col2, r2, m);
		}
	}

	bool CollisionIntersection_Sub_Box(Collider& col1, const RigidBody& r1,Collider& col2, const RigidBody& r2, Manifold& manifold)
	{
		ColliderType type2 = col2.type;
		switch (type2)
		{
		case ColliderType::BOX:
		{
			if (CollisionIntersection_RectRect(col1, r1.Velocity, col2, r2.Velocity))
			{
				AABBvsAABB_Manifold(manifold);
				return true;
			}
			return false;
		}
		case ColliderType::CIRCLE:
			return CollisionIntersection_RectCircle_Static(col1.center, col1.size, col2.center, col2.size,manifold);
		case ColliderType::LINE:
			return CollisionIntersection_RectLine_Static(col1.center, col1.size, col2.center, col2.size);
		case ColliderType::RAY:
			return CollisionIntersection_RectRay_Static(col1.center, col1.size, col2.center, col2.size);
		default:
			return CollisionIntersection_RectRect_Static(col1.center, col1.size, col2.center, col2.size);
		}
	}
	bool CollisionIntersection_Sub_Circle(Collider& col1, const RigidBody& r1,Collider& col2, const RigidBody& r2,Manifold& manifold)
	{
		UNREFERENCED_PARAMETER(r1);
		UNREFERENCED_PARAMETER(r2);
		float intertime = 0;
		ColliderType type2 = col2.type;
		switch (type2)
		{
		case ColliderType::BOX:
			return CollisionIntersection_RectCircle_Static(col2.center, col2.size, col1.center, col1.size,manifold);
		case ColliderType::CIRCLE:
		{
			//return CollisionIntersection_CircleCircle_Static(col1.center, col1.size, col2.center, col2.size);
			return CollisionIntersection_CircleCircle(col1, r1.Velocity, col2, r2.Velocity,manifold);
		}
		case ColliderType::LINE:
			return CollisionIntersection_CircleLineSegment_Static(col1.center, col1.size, col2.center, col2.size);
		case ColliderType::RAY:
		{
			Ray ray{ col1.center,r1.Velocity };
			return CollisionIntersection_RayCircle(ray, col2,manifold);
		}
		default:
			return CollisionIntersection_RectCircle_Static(col2.center, col2.size, col1.center, col1.size,manifold);
		}
	}
	bool CollisionIntersection_Sub_Line(Collider& col1,Collider& col2, Manifold& manifold)
	{
		UNREFERENCED_PARAMETER(manifold);
		ColliderType type2 = col2.type;
		switch (type2)
		{
		case ColliderType::BOX:
			return CollisionIntersection_RectLine_Static(col2.center, col2.size, col1.center, col1.size);
		case ColliderType::CIRCLE:
			return CollisionIntersection_CircleLineSegment_Static(col2.center, col2.size, col1.center, col1.size);
		case ColliderType::LINE:
			return CollisionIntersection_LineLine_Static(col1.center, col1.size, col2.center, col2.size);
		case ColliderType::RAY:
			return CollisionIntersection_RayLine_Static(col2.center, col2.size, col1.center, col1.size);
		default:
			return CollisionIntersection_RectLine_Static(col2.center, col2.size, col1.center, col1.size);
		}
	}
	bool CollisionIntersection_Sub_Ray(Collider& col1,const RigidBody& r1,Collider& col2,const RigidBody& r2,Manifold& m)
	{
		UNREFERENCED_PARAMETER(r1);
		UNREFERENCED_PARAMETER(r2);
		ColliderType type2 = col2.type;
		switch (type2)
		{
		case ColliderType::BOX:
			return CollisionIntersection_RectRay_Static(col2.center, col2.size, col1.center, col1.size);
		case ColliderType::CIRCLE:
		{
			Ray ray{ col1.center,r1.Velocity };
			return CollisionIntersection_RayCircle(ray, col2,m);
		}
		case ColliderType::LINE:
			return CollisionIntersection_RayLine_Static(col1.center, col1.size, col2.center, col2.size);
		case ColliderType::RAY:
			return false; //to be done
		default:
			return CollisionIntersection_RectRay_Static(col2.center, col2.size, col1.center, col1.size);
		}
	}

	bool AABBvsAABB_Manifold(Manifold& m)
	{
		Collider A = m.A;
		Collider B = m.B;

		// Vector from A to B
		Vector2 n = A.center - B.center;

		AABB abox{ A.center,A.size };
		AABB bbox{ B.center,B.size };

		// Calculate half extents along x axis for each object
		float a_extent = (abox.max.x - abox.min.x) / 2;
		float b_extent = (bbox.max.x - bbox.min.x) / 2;

		// Calculate overlap on x axis
		float x_overlap = a_extent + b_extent - abs(n.x);

		// SAT test on x axis
		if (x_overlap > 0)
		{
			// Calculate half extents along x axis for each object
			float a_extent2 = (abox.max.y - abox.min.y) / 2;
			float b_extent2 = (bbox.max.y - bbox.min.y) / 2;

			// Calculate overlap on y axis
			float y_overlap = a_extent2 + b_extent2 - abs(n.y);

				// SAT test on y axis
				if (y_overlap > 0)
				{
					// Find out which axis is axis of least penetration
					if (x_overlap > y_overlap)
					{
						// Point towards B knowing that n points from A to B
						if (n.x < 0)
							m.normal = Vector2{ -1,0 };
						else
							m.normal = Vector2{ 0,0 };
							m.penetration = x_overlap;
							return true;
					}
					else
					{
						// Point toward B knowing that n points from A to B
						if (n.y < 0)
							m.normal = Vector2{ 0,-1 };
						else
							m.normal = Vector2{ 0, 1 };
							m.penetration = y_overlap;
							return true;
					}
				}
		}
		return false;
	}

}

//UNUSED FUNCTIONS

	/******************************************************************************/
	/*!
		Checks for collision between a circle and a line segment
	 */
	 /******************************************************************************/
//int CollisionIntersection_CircleLineSegment(const Circle& circle,
//	const  Vector2& ptEnd,
//	LineSegment& lineSeg,
//	Vector2& interPt,
//	Vector2& normalAtCollision,
//	float& interTime,
//	bool& checkLineEdges)
//{
//	float DP_FirstPoint = lineSeg.m_normal.DotProduct(circle.m_center); //N.Bs
//	float DP_LineSeg = lineSeg.m_normal.DotProduct(lineSeg.m_pt0);      //N.P0
//
//	//Circle hits body of Line Segment opposite the direction of the normal
//	if (DP_FirstPoint - DP_LineSeg <= -circle.m_radius)
//	{
//		Vector2 newP0 = lineSeg.m_pt0 - lineSeg.m_normal * circle.m_radius;
//		Vector2 newP1 = lineSeg.m_pt1 - lineSeg.m_normal * circle.m_radius;
//
//		//Find velocity of the ball
//		Vector2 V = (ptEnd - circle.m_center);
//
//		//Find Outward Normal
//		Vector2 M;
//		M.x = V.y;
//		M.y = -V.x;
//
//		//Checks the distance of the ball to the line segment
//		if (M.DotProduct(newP0 - circle.m_center) * M.DotProduct(newP1 - circle.m_center) < 0)
//		{
//			//Calculate time of intersection
//			interTime = (DP_LineSeg - DP_FirstPoint - circle.m_radius) / lineSeg.m_normal.DotProduct(V);
//
//			if (interTime >= 0 && interTime <= 1)
//			{
//				//Ball has collided, set intersection point, calculate normal and return true
//				interPt = circle.m_center + V * interTime;
//				normalAtCollision = -lineSeg.m_normal;
//				return 1;
//			}
//		}
//		else
//		{
//			//Ball is NOT within both lines
//			if (checkLineEdges)
//				return CheckMovingCircleToLineEdge(false, circle, ptEnd, lineSeg, interPt, normalAtCollision, interTime);
//		}
//	}
//
//	//Circle hits body of Line Segment in the direction of the normal
//	else if (DP_FirstPoint - DP_LineSeg >= circle.m_radius)
//	{
//		Vector2 newP0 = lineSeg.m_pt0 + lineSeg.m_normal * circle.m_radius;
//		Vector2 newP1 = lineSeg.m_pt1 + lineSeg.m_normal * circle.m_radius;
//
//		//Find Outward Normal
//		Vector2 V = (ptEnd - circle.m_center);
//
//		//Find Outward Normal
//		Vector2 M;
//		M.x = V.y;
//		M.y = -V.x;
//
//		//Checks the distance of the ball to the line segment
//		if (M.DotProduct(newP0 - circle.m_center) * M.DotProduct(newP1 - circle.m_center) < 0)
//		{
//			//Calculate time of intersection
//			interTime = (DP_LineSeg - DP_FirstPoint + circle.m_radius) / lineSeg.m_normal.DotProduct(V);
//
//			if (interTime >= 0 && interTime <= 1)
//			{
//				//Ball has collided, set intersection point, calculate normal and return true
//				interPt = circle.m_center + V * interTime;
//				normalAtCollision = lineSeg.m_normal;
//				return 1;
//			}
//		}
//		else
//		{
//			//Ball is NOT within both lines
//			if (checkLineEdges)
//				return CheckMovingCircleToLineEdge(false, circle, ptEnd, lineSeg, interPt, normalAtCollision, interTime);
//		}
//	}
//	else
//	{
//		//Ball is within both lines
//		if (checkLineEdges)
//			return CheckMovingCircleToLineEdge(true, circle, ptEnd, lineSeg, interPt, normalAtCollision, interTime);
//	}
//	// no intersection
//	return 0;
//
//
//}

	/******************************************************************************/
	/*!
		Checks to see if the ball bumps onto any edges
	*/
	/******************************************************************************/
	//int CheckMovingCircleToLineEdge(bool withinBothLines,
	//	const Circle& circle,
	//	const Vector2& ptEnd,
	//	const LineSegment& lineSeg,
	//	Vector2& interPt,
	//	Vector2& normalAtCollision,
	//	float& interTime)
	//{
	//	//Find velocity of the ball
	//	Vector2 V = (ptEnd - circle.m_center);
	//	Vector2 Vnormalised = V.Normalize();
	//
	//	//Find Outward Normal
	//	Vector2 M;
	//	M.x = V.y;
	//	M.y = -V.x;
	//	M = M.Normalize();
	//	float m0 = Vnormalised.DotProduct(lineSeg.m_pt0 - circle.m_center); //BsP0.v
	//	float m1 = Vnormalised.DotProduct(lineSeg.m_pt1 - circle.m_center); //BsP1.v
	//
	//	//is the center of circle within both lines
	//	if (withinBothLines == true)
	//	{
	//		if ((lineSeg.m_pt0 - circle.m_center).DotProduct(lineSeg.m_pt1 - lineSeg.m_pt0) > 0) //P0 Side
	//		{
	//			float dist0 = M.DotProduct(lineSeg.m_pt0 - circle.m_center); //BsP0.M
	//			if (m0 > 0) //otherwise no collision
	//			{
	//				if (abs(dist0) > circle.m_radius)
	//				{
	//					return 0;
	//				}
	//
	//				float H = sqrtf(circle.m_radius * circle.m_radius - dist0 * dist0);
	//				interTime = (m0 - H) / V.Magnitude();
	//				if (interTime <= 1)
	//				{
	//					//Calculate intersection and normal for reflection
	//					interPt = circle.m_center + V * interTime;
	//					normalAtCollision = interPt - lineSeg.m_pt0;
	//					normalAtCollision = normalAtCollision.Normalize();
	//					return 1;
	//				}
	//			}
	//
	//		}
	//		else //P1 Side
	//		{
	//			if (m1 > 0)
	//			{
	//				float dist1 = static_cast<float>(M.DotProduct(lineSeg.m_pt1 - circle.m_center)); //BsP1.M
	//				if (abs(dist1) > circle.m_radius)
	//				{
	//					return 0;
	//				}
	//				float H = static_cast<float>(sqrt(circle.m_radius * circle.m_radius - dist1 * dist1));
	//				interTime = (m1 - H) / V.Magnitude();
	//				if (interTime <= 1)
	//				{
	//					//Calculate intersection and normal for reflection
	//					interPt = circle.m_center + V * interTime;
	//					normalAtCollision = interPt - lineSeg.m_pt1;
	//					normalAtCollision = normalAtCollision.Normalize();
	//					return 1;
	//				}
	//			}
	//		}
	//
	//	}
	//	else //if not within both lines
	//	{
	//		//check to see if it is P0 side or P1 side
	//		bool P0side = false;
	//		float dist0 = abs(M.DotProduct(lineSeg.m_pt0 - circle.m_center));
	//		float dist1 = abs(M.DotProduct(lineSeg.m_pt1 - circle.m_center));
	//
	//		if (dist0 > circle.m_radius&& dist1 > circle.m_radius)
	//		{
	//			return 0;
	//		}
	//
	//		if (dist0 <= circle.m_radius && dist1 <= circle.m_radius)
	//		{
	//			float dist2 = abs(Vnormalised.DotProduct(lineSeg.m_pt0 - circle.m_center));
	//			float dist3 = abs(Vnormalised.DotProduct(lineSeg.m_pt1 - circle.m_center));
	//			P0side = (dist2 < dist3) ? true : false;
	//		}
	//		else if (dist0 <= circle.m_radius)
	//		{
	//			P0side = true;
	//		}
	//		else
	//		{
	//			P0side = false;
	//		}
	//		//-----------------------------------------
	//
	//		if (P0side)//if on P0 side
	//		{
	//			if (m0 < 0)
	//			{
	//				return 0;
	//			}
	//			else
	//			{
	//				float H = static_cast<float>(sqrt(circle.m_radius * circle.m_radius - dist0 * dist0));
	//				interTime = static_cast<float>((m0 - H) / V.Magnitude());
	//				if (interTime <= 1)
	//				{
	//					//Calculate intersection and normal for reflection
	//					interPt = circle.m_center + V * interTime;
	//					normalAtCollision = interPt - lineSeg.m_pt0;
	//					normalAtCollision = normalAtCollision.Normalize();
	//					return 1;
	//				}
	//			}
	//		}
	//		else //if on P1 side
	//		{
	//			if (m1 < 0)
	//			{
	//				return 0;
	//			}
	//			else
	//			{
	//				float H = static_cast<float>(sqrt(circle.m_radius * circle.m_radius - dist1 * dist1));
	//				interTime = static_cast<float>((m1 - H) / V.Magnitude());
	//				if (interTime <= 1)
	//				{
	//					//Calculate intersection and normal for reflection
	//					interPt = circle.m_center + V * interTime;
	//					normalAtCollision = interPt - lineSeg.m_pt1;
	//					normalAtCollision = normalAtCollision.Normalize();
	//					return 1;
	//				}
	//			}
	//		}
	//	}
	//
	//	return 0;
	//}

