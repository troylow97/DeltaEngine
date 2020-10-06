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
#include "Core/Math/Vector.h"
#include "Physics/CollisionShapes.h"

namespace DeltaEngine
{
	bool CollisionIntersection_RectRect(const AABB& aabb1, const Vector2& vel1,
		const AABB& aabb2, const Vector2& vel2);

	void BuildLineSegment(LineSegment& lineSegment,
		const  Vector2& pos,
		float scale,
		float dir);

	int CollisionIntersection_CircleLineSegment(const Circle& circle,
		const Vector2& ptEnd,
		const LineSegment& lineSeg,
		Vector2& interPt,
		Vector2& normalAtCollision,
		float& interTime,
		bool& checkLineEdges);

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

	int CollisionIntersection_RayCircle(const Ray& ray,
		const Circle& circle,
		float& interTime);

	bool CollisionIntersection_RayLine_Static(const Ray& ray, const LineSegment& line);

	bool CollisionIntersection_LineLine_Static(const LineSegment& line, const LineSegment& line2);

	bool CollisionIntersecction_CircleCircle_Static(const Circle& circle1, const Circle& circle2);

	bool CollisionIntersection_CircleLineSegment_Static(const Circle& circle, const LineSegment& line);

	bool CollisionIntersection_RectLine_Static(const AABB& aabb, const LineSegment& line);

	bool CollisionIntersection_RectCircle_Static(const AABB& aabb1, const Circle& circle);

	bool CollisionIntersection_RectRay_Static(const AABB& aabb, Ray r);

	bool CollisionIntersection_RectRect_Static(const AABB& aabb1, const AABB& aabb2);

}