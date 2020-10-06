#pragma once
#include "Core/Math/Vector.h"
#include "Physics/CollisionShapes.h"
#include "Collider.h"
 
namespace DeltaEngine
{
	void BuildLineSegment(LineSegment& lineSegment,
		const  Vector2& pos,
		float scale,
		float dir);
//COLLISION TESTS
	bool CollisionIntersection_RectRect_Static(const Vector2 Center1, const Vector2 Size1, const Vector2 Center2, const Vector2 Size2);

	bool CollisionIntersection_RectRay_Static(const Vector2 Center1, const Vector2 Size1, const Vector2 Center2, const Vector2 Size2);

	bool CollisionIntersection_RectCircle_Static(const Vector2 Center1, const Vector2 Size1, const Vector2 Center2, const Vector2 Size2);

	bool CollisionIntersection_RectLine_Static(const Vector2 Center1, const Vector2 Size1, const Vector2 Center2, const Vector2 Size2);

	bool CollisionIntersection_CircleCircle_Static(const Vector2 Center1, const Vector2 Size1, const Vector2 Center2, const Vector2 Size2);

	bool CollisionIntersection_CircleLineSegment_Static(const Vector2 Center1, const Vector2 Size1, const Vector2 Center2, const Vector2 Size2);

	bool CollisionIntersection_RayLine_Static(const Vector2 Center1, const Vector2 Size1, const Vector2 Center2, const Vector2 Size2);

	bool CollisionIntersection_LineLine_Static(const Vector2 Center1, const Vector2 Size1, const Vector2 Center2, const Vector2 Size2);

	int CollisionIntersection_RayCircle(const Vector2 Center1, const Vector2 Size1, const Vector2 Center2, const Vector2 Size2, float& interTime);

	bool CollisionIntersection_Main(const Collider&,const Collider&);

	bool CollisionIntersection_Sub_Box(const Collider&,const Collider&);

	bool CollisionIntersection_Sub_Circle(const Collider&,const Collider&);

	bool CollisionIntersection_Sub_Ray(const Collider&,const Collider&);

	bool CollisionIntersection_Sub_Line(const Collider&,const Collider&);
//UNUSED FUNCTIONS

	int CollisionIntersection_CircleLineSegment(const Circle& circle,
		const Vector2& ptEnd,
		const LineSegment& lineSeg,
		Vector2& interPt,
		Vector2& normalAtCollision,
		float& interTime,
		bool& checkLineEdges);

	//Need to add
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

}