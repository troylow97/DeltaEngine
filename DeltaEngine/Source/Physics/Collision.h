/* Start Header******************************************************************/
/*!
\file		Collision.h
\author 	CHIN Mei Qi, Clara, m.chin, 440004919
\par    	email: m.chin\@digipen.edu
\date   	March 11, 2020
\brief      This file is the header file for Collision.cpp

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /* End Header *******************************************************************/
#pragma once
#include "Math/Vector.h"
#include "CollisionShapes.h"
#include "DEpch.h"
namespace DeltaEngine
{
	void BuildLineSegment(LineSegment& lineSegment,
		const  Vector2& pos,
		float scale,
		float dir);

	bool CollisionIntersection_RectRect_Static(const AABB& aabb1, const AABB& aabb2);

    bool CollisionIntersection_RectRect(const AABB& aabb1, const Vector2& vel1,
        const AABB& aabb2, const Vector2& vel2);

	bool CollisionIntersection_RectCircle_Static(const AABB& aabb1, const Circle& circle);

	bool CollisionIntersection_RectLine_Static(const AABB& aabb1, const LineSegment& line);

	int CollisionIntersection_CircleLineSegment(const Circle& circle,		
		const Vector2& ptEnd,												
		const LineSegment& lineSeg,											
		Vector2& interPt,													
		Vector2& normalAtCollision,											
		float& interTime,													
		bool& checkLineEdges);

	bool CollisionIntersection_CircleLineSegment_Static(const Circle& circle, const LineSegment& line);

	int CheckMovingCircleToLineEdge(bool withinBothLines,
		const Circle& circle,							
		const Vector2& ptEnd,
		const LineSegment& lineSeg,						
		Vector2& interPt,							
		Vector2& normalAtCollision,					
		float& interTime);

	int CollisionIntersection_CircleCircle(const Circle& circleA,					
		const Vector2& velA,													
		const Circle& circleB,														
		const Vector2& velB,
		Vector2& interPtA,														
		Vector2& interPtB,														
		float& interTime);

	bool CollisionIntersecction_CircleCircle_Static(const Circle& circle1, const Circle& circle2);

	int CollisionIntersection_RayCircle(const Ray& ray,								
		const Circle& circle,														
		float& interTime);																							

	bool CollisionIntersection_RectRay_Static(const AABB& aabb, Ray r);

	bool CollisionIntersection_RayLine_Static(const Ray& ray, const LineSegment& line);

	bool CollisionIntersection_LineLine_Static(const LineSegment& line1, const LineSegment& line2);

	void CollisionResponse_CircleCircle(Vector2& normal,						
		const float interTime,													
		Vector2& velA,															
		const float& massA,														
		Vector2& interPtA,														
		Vector2& velB,															
		const float& massB,														
		Vector2& interPtB,														
		Vector2& reflectedVectorA,												
		Vector2& ptEndA,														
		Vector2& reflectedVectorB,												
		Vector2& ptEndB);														

}
