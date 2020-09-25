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
#include "Collision.h"
#include <algorithm>

namespace DeltaEngine
{
	float g_dt; //to be removed

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

	/**************************************************************************/
	/*!
	  \brief	Test for intersection between two rectangular bounded objects
	  \param	aabb1 AABB of the first object
	  \param	vel1  Velocity of the first object
	  \param	aabb2 AABB of the second object
	  \param	vel2  Velocity of the second object
	*/
	/**************************************************************************/
	bool CollisionIntersection_RectRect(const AABB& aabb1, const Vector2& vel1,
		const AABB& aabb2, const Vector2& vel2)
	{
		//Static Collision Check
		if (!((aabb1.max.x < aabb2.min.x) || (aabb1.min.x > aabb2.max.x) || (aabb1.max.y < aabb2.min.y) || (aabb1.min.y > aabb2.max.y)))
			return true;
	
		float tFirst = 0;
		float tLast = g_dt;
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
				tFirst = max((aabb1.max.x - aabb2.min.x) / RelativeVel.x, tFirst);
			if (aabb1.min.x < aabb2.max.x) // check A min < B Max
				tLast = min((aabb1.min.x - aabb2.max.x) / RelativeVel.x, tLast);
		}
		else if (RelativeVel.x > 0.0f)  //caculate the x axis of the AABB when RelativeVel is < 0
		{
			// checking for case 2
			if (aabb1.max.x < aabb2.min.x) //check A max < B min
				return false;
			if (aabb1.min.x > aabb2.max.x) //check A min > B max
				tFirst = max((aabb1.min.x - aabb2.max.x) / RelativeVel.x, tFirst);
			if (aabb1.max.x > aabb2.min.x) //check A min > B min
				tLast = min((aabb1.max.x - aabb2.min.x) / RelativeVel.x, tLast);
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
				tFirst = max((aabb1.max.y - aabb2.min.y) / RelativeVel.y, tFirst);
			if (aabb1.min.y < aabb2.max.y) // check A min < B Max
				tLast = min((aabb1.min.y - aabb2.max.y) / RelativeVel.y, tLast);
		}
		else if (RelativeVel.y > 0.0f)
		{
			// checking for case 2
			if (aabb1.max.y < aabb2.min.y) //check A max < B min
				return false;
			if (aabb1.min.y > aabb2.max.y) //check A min > B max
				tFirst = max((aabb1.min.y - aabb2.max.y) / RelativeVel.y, tFirst);
			if (aabb1.max.y > aabb2.min.y) //check A min > B min
				tLast = min((aabb1.max.y - aabb2.min.y) / RelativeVel.y, tLast);
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

	bool CollisionIntersection_RectRect_Static(const AABB& aabb1, const AABB& aabb2)
	{
		if (!((aabb1.max.x < aabb2.min.x) || (aabb1.min.x > aabb2.max.x) || (aabb1.max.y < aabb2.min.y) || (aabb1.min.y > aabb2.max.y)))
			return true;

		return false;
	}

	//need to test
	bool CollisionIntersection_RectRay_Static(const AABB& aabb,Ray r)
	{
		float tx1 = (aabb.min.x - r.m_pt0.x) * r.m_dir.x;
		float tx2 = (aabb.max.x - r.m_pt0.x) * r.m_dir.x;

		float tmin = min(tx1, tx2);
		float tmax = max(tx1, tx2);

		float ty1 = (aabb.min.y - r.m_pt0.y) * r.m_dir.y;
		float ty2 = (aabb.max.y - r.m_pt0.y) * r.m_dir.y;

		tmin = max(tmin, min(ty1, ty2));
		tmax = min(tmax, max(ty1, ty2));

		return tmax >= tmin;
	}

	bool CollisionIntersection_RectCircle_Static(const AABB& aabb1, const Circle& circle)
	{
		// temporary variables to set edges for testing

		Vector2 size;
		Vector2 center;
		size.x = aabb1.max.x - aabb1.min.x;
		size.y = aabb1.max.y - aabb1.min.y;
		center.x = aabb1.min.x + 0.5 * size.x;
		center.y = aabb1.max.x + 0.5 * size.y;
		Vector2 temp_vec = center;
		// which edge is closest?
		if (circle.m_center.x < center.x)         temp_vec.x = center.x;                  // left edge
		else if (circle.m_center.x > center.x + size.x) temp_vec.x = center.x + size.x;   // right edge
		if (circle.m_center.y < center.y)         temp_vec.y = center.y;                  // top edge
		else if (circle.m_center.y > center.y + size.y) temp_vec.y = center.y + size.y;   // bottom edge

		// get distance from closest edges
		Vector2 dist = { center.x - temp_vec.x,center.y - temp_vec.y };
		float distance = sqrt((dist.x * dist.x) + (dist.y * dist.y));

		// if the distance is less than the radius, collision!
		if (distance <= circle.m_radius)
			return true;

		return false;
	}

	bool CollisionIntersection_RectLine_Static(const AABB& aabb, const LineSegment& line)
	{
		if (((line.m_pt0.x > aabb.min.x&& line.m_pt0.x < aabb.max.x) || (line.m_pt1.x > aabb.min.x&& line.m_pt1.x < aabb.max.x)) &&
			((line.m_pt0.y > aabb.min.y&& line.m_pt0.y < aabb.max.y) || (line.m_pt1.y > aabb.min.y&& line.m_pt1.y < aabb.max.y)))
		{
			return true;
		}

		return false;
	}

	/******************************************************************************/
	/*!
		Checks for collision between a circle and a line segment
	 */
	 /******************************************************************************/
	int CollisionIntersection_CircleLineSegment(const Circle& circle,
		const  Vector2& ptEnd,
		LineSegment& lineSeg,
		Vector2& interPt,
		Vector2& normalAtCollision,
		float& interTime,
		bool& checkLineEdges)
	{
		float DP_FirstPoint = lineSeg.m_normal.DotProduct(circle.m_center); //N.Bs
		float DP_LineSeg = lineSeg.m_normal.DotProduct(lineSeg.m_pt0);      //N.P0

		//Circle hits body of Line Segment opposite the direction of the normal
		if (DP_FirstPoint - DP_LineSeg <= -circle.m_radius)
		{
			Vector2 newP0 = lineSeg.m_pt0 - lineSeg.m_normal * circle.m_radius;
			Vector2 newP1 = lineSeg.m_pt1 - lineSeg.m_normal * circle.m_radius;

			//Find velocity of the ball
			Vector2 V = (ptEnd - circle.m_center);

			//Find Outward Normal
			Vector2 M;
			M.x = V.y;
			M.y = -V.x;

			//Checks the distance of the ball to the line segment
			if (M.DotProduct(newP0 - circle.m_center) * M.DotProduct(newP1 - circle.m_center) < 0)
			{
				//Calculate time of intersection
				interTime = (DP_LineSeg - DP_FirstPoint - circle.m_radius) / lineSeg.m_normal.DotProduct(V);

				if (interTime >= 0 && interTime <= 1)
				{
					//Ball has collided, set intersection point, calculate normal and return true
					interPt = circle.m_center + V * interTime;
					normalAtCollision = -lineSeg.m_normal;
					return 1;
				}
			}
			else
			{
				//Ball is NOT within both lines
				if (checkLineEdges)
					return CheckMovingCircleToLineEdge(false, circle, ptEnd, lineSeg, interPt, normalAtCollision, interTime);
			}
		}

		//Circle hits body of Line Segment in the direction of the normal
		else if (DP_FirstPoint - DP_LineSeg >= circle.m_radius)
		{
			Vector2 newP0 = lineSeg.m_pt0 + lineSeg.m_normal * circle.m_radius;
			Vector2 newP1 = lineSeg.m_pt1 + lineSeg.m_normal * circle.m_radius;

			//Find Outward Normal
			Vector2 V = (ptEnd - circle.m_center);

			//Find Outward Normal
			Vector2 M;
			M.x = V.y;
			M.y = -V.x;

			//Checks the distance of the ball to the line segment
			if (M.DotProduct(newP0 - circle.m_center) * M.DotProduct(newP1 - circle.m_center) < 0)
			{
				//Calculate time of intersection
				interTime = (DP_LineSeg - DP_FirstPoint + circle.m_radius) / lineSeg.m_normal.DotProduct(V);

				if (interTime >= 0 && interTime <= 1)
				{
					//Ball has collided, set intersection point, calculate normal and return true
					interPt = circle.m_center + V * interTime;
					normalAtCollision = lineSeg.m_normal;
					return 1;
				}
			}
			else
			{
				//Ball is NOT within both lines
				if (checkLineEdges)
					return CheckMovingCircleToLineEdge(false, circle, ptEnd, lineSeg, interPt, normalAtCollision, interTime);
			}
		}
		else
		{
			//Ball is within both lines
			if (checkLineEdges)
				return CheckMovingCircleToLineEdge(true, circle, ptEnd, lineSeg, interPt, normalAtCollision, interTime);
		}
		// no intersection
		return 0;


	}

	bool CollisionIntersection_CircleLineSegment_Static(const Circle& circle, const LineSegment& line)
	{
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
		Checks to see if the ball bumps onto any edges
	*/
	/******************************************************************************/
	int CheckMovingCircleToLineEdge(bool withinBothLines,
		const Circle& circle,
		const Vector2& ptEnd,
		const LineSegment& lineSeg,
		Vector2& interPt,
		Vector2& normalAtCollision,
		float& interTime)
	{
		//Find velocity of the ball
		Vector2 V = (ptEnd - circle.m_center);
		Vector2 Vnormalised = V.Normalize();

		//Find Outward Normal
		Vector2 M;
		M.x = V.y;
		M.y = -V.x;
		M = M.Normalize();
		float m0 = Vnormalised.DotProduct(lineSeg.m_pt0 - circle.m_center); //BsP0.v
		float m1 = Vnormalised.DotProduct(lineSeg.m_pt1 - circle.m_center); //BsP1.v

		//is the center of circle within both lines
		if (withinBothLines == true)
		{
			if ((lineSeg.m_pt0 - circle.m_center).DotProduct(lineSeg.m_pt1 - lineSeg.m_pt0) > 0) //P0 Side
			{
				float dist0 = M.DotProduct(lineSeg.m_pt0 - circle.m_center); //BsP0.M
				if (m0 > 0) //otherwise no collision
				{
					if (abs(dist0) > circle.m_radius)
					{
						return 0;
					}

					float H = sqrtf(circle.m_radius * circle.m_radius - dist0 * dist0);
					interTime = (m0 - H) / V.Magnitude();
					if (interTime <= 1)
					{
						//Calculate intersection and normal for reflection
						interPt = circle.m_center + V * interTime;
						normalAtCollision = interPt - lineSeg.m_pt0;
						normalAtCollision = normalAtCollision.Normalize();
						return 1;
					}
				}

			}
			else //P1 Side
			{
				if (m1 > 0)
				{
					float dist1 = static_cast<float>(M.DotProduct(lineSeg.m_pt1 - circle.m_center)); //BsP1.M
					if (abs(dist1) > circle.m_radius)
					{
						return 0;
					}
					float H = static_cast<float>(sqrt(circle.m_radius * circle.m_radius - dist1 * dist1));
					interTime = (m1 - H) / V.Magnitude();
					if (interTime <= 1)
					{
						//Calculate intersection and normal for reflection
						interPt = circle.m_center + V * interTime;
						normalAtCollision = interPt - lineSeg.m_pt1;
						normalAtCollision =  normalAtCollision.Normalize();
						return 1;
					}
				}
			}

		}
		else //if not within both lines
		{
			//check to see if it is P0 side or P1 side
			bool P0side = false;
			float dist0 = abs(M.DotProduct(lineSeg.m_pt0 - circle.m_center));
			float dist1 = abs(M.DotProduct(lineSeg.m_pt1 - circle.m_center));

			if (dist0 > circle.m_radius&& dist1 > circle.m_radius)
			{
				return 0;
			}

			if (dist0 <= circle.m_radius && dist1 <= circle.m_radius)
			{
				float dist2 = abs(Vnormalised.DotProduct(lineSeg.m_pt0 - circle.m_center));
				float dist3 = abs(Vnormalised.DotProduct(lineSeg.m_pt1 - circle.m_center));
				P0side = (dist2 < dist3) ? true : false;
			}
			else if (dist0 <= circle.m_radius)
			{
				P0side = true;
			}
			else
			{
				P0side = false;
			}
			//-----------------------------------------

			if (P0side)//if on P0 side
			{
				if (m0 < 0)
				{
					return 0;
				}
				else
				{
					float H = static_cast<float>(sqrt(circle.m_radius * circle.m_radius - dist0 * dist0));
					interTime = static_cast<float>((m0 - H) / V.Magnitude());
					if (interTime <= 1)
					{
						//Calculate intersection and normal for reflection
						interPt = circle.m_center + V * interTime;
						normalAtCollision = interPt - lineSeg.m_pt0;
						normalAtCollision = normalAtCollision.Normalize();
						return 1;
					}
				}
			}
			else //if on P1 side
			{
				if (m1 < 0)
				{
					return 0;
				}
				else
				{
					float H = static_cast<float>(sqrt(circle.m_radius * circle.m_radius - dist1 * dist1));
					interTime = static_cast<float>((m1 - H) / V.Magnitude());
					if (interTime <= 1)
					{
						//Calculate intersection and normal for reflection
						interPt = circle.m_center + V * interTime;
						normalAtCollision = interPt - lineSeg.m_pt1;
						normalAtCollision = normalAtCollision.Normalize();
						return 1;
					}
				}
			}
		}

		return 0;
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
	int CollisionIntersection_CircleCircle(const Circle& circleA,
		const Vector2& velA,
		const Circle& circleB,
		const Vector2& velB,
		Vector2& interPtA,
		Vector2& interPtB,
		float& interTime)
	{
		//Get relative velocity by considering circleB as non-moving by taking velA - velB
		//Use the circle as a point for the ray
		Ray ray{ circleA.m_center, velA - velB };

		//Use a circles center with a combined radius of both circles
		Circle newCircle{ circleB.m_center, circleB.m_radius + circleA.m_radius };

		if (CollisionIntersection_RayCircle(ray, newCircle, interTime))
		{
			//intersection when both circle collides
			interPtA = circleA.m_center + velA * interTime;
			interPtB = circleB.m_center + velB * interTime;
			return 1;
		}
		return 0;
	}

	bool CollisionIntersecction_CircleCircle_Static(const Circle& circle1, const Circle& circle2)
	{
		//(x2 - x1) ^ 2 + (y1 - y2) ^ 2 <= (r1 + r2) ^ 2
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

	/******************************************************************************/
	/*!
		Calculates if a ray and a circle intersects.
		If it does, it changes the interTime parameter
	 */
	 /******************************************************************************/
	int CollisionIntersection_RayCircle(const Ray& ray,
		const Circle& circle,
		float& interTime)
	{
		//Calculate end point of ray
		Vector2 Be = ray.m_pt0 + ray.m_dir;

		//ray.m_dir is the velocity, use it to get its normalised version
		Vector2 normalisedVelocity = ray.m_dir.normalized(); //CHECK

		Vector2 BsC = circle.m_center - ray.m_pt0;

		// check if circle is behind ray origin
		float m = BsC.DotProduct(normalisedVelocity);
		if ((m < 0) && (BsC.Magnitude() >= circle.m_radius))
		{
			return 0;
		}

		// check if ray within circle radius range
		float n2 = BsC.Magnitude() - (m * m);
		if (n2 <= (circle.m_radius * circle.m_radius))
		{
			float s2 = sqrtf((circle.m_radius * circle.m_radius) - n2);
			float rayLength = ray.m_dir.Magnitude();
			float ti0 = (m - s2) / rayLength;
			float ti1 = (m + s2) / rayLength;

			if (ti0 > ti1)
				interTime = ti1;
			else
				interTime = ti0;

			if (interTime > 0 && interTime < 1)
				return 1;
		}
		return 0;
	}

	bool CollisionIntersection_RayLine_Static(const Ray& ray, const LineSegment& line)
	{
		Vector2 v1 = ray.m_pt0 - line.m_pt0;
		Vector2 v2 = line.m_pt1 - line.m_pt0;
		Vector2 v3 = Vector2(-ray.m_dir.y, ray.m_dir.x);

		float dot = v2.DotProduct(v3);
		if (abs(dot) < 0.000001)
			return -1.0f;

		float t1 = v2.CrossProduct_Magnitude(v1) / dot;
		float t2 = v1.DotProduct(v3) / dot;

		if (t1 >= 0.0 && (t2 >= 0.0 && t2 <= 1.0))
			return t1;

		return -1.0f;
	}

	bool CollisionIntersection_LineLine_Static(const LineSegment& line, const LineSegment& line2)
	{
		float denominator = ((line.m_pt1.x - line.m_pt0.x) * (line2.m_pt1.y - line2.m_pt0.y)) - ((line.m_pt1.y - line.m_pt0.y) * (line2.m_pt1.x - line2.m_pt0.x));
		float numerator1 = ((line.m_pt0.y - line2.m_pt0.y) * (line2.m_pt1.x - line2.m_pt0.x)) - ((line.m_pt0.x - line2.m_pt0.x) * (line2.m_pt1.y - line2.m_pt0.y));
		float numerator2 = ((line.m_pt0.y - line2.m_pt0.y) * (line.m_pt1.x - line.m_pt0.x)) - ((line.m_pt0.x - line2.m_pt0.x) * (line.m_pt1.y - line.m_pt0.y));

		// Detect coincident lines (has a problem)
		if (denominator == 0) return numerator1 == 0 && numerator2 == 0;

		float r = numerator1 / denominator;
		float s = numerator2 / denominator;

		return (r >= 0 && r <= 1) && (s >= 0 && s <= 1);
	}
	/******************************************************************************/
	/*!
		Calculates reflection when the circle hits a line.
		A reflection vector is calculated and the end point is also updated.
	 */
	 /******************************************************************************/
	void CollisionResponse_CircleLineSegment(const  Vector2& interPt,
		const  Vector2& normal,
		Vector2& ptEnd,
		Vector2& reflected)
	{
		//penetration is how much excess your velocity will go through
		Vector2 penetration = ptEnd - interPt;

		//calculate the reflection vector based on direction of ball and normal
		float temp = penetration.DotProduct(normal);
		reflected = penetration - normal * 2 * temp;

		//update ptEnd which is the next position of the ball
		ptEnd = interPt + reflected;
		reflected = reflected.Normalize();
	}
}
