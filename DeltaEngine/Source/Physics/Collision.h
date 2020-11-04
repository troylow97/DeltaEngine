#pragma once
#include "Physics/CollisionShapes.h"
#include "Manifold.h"
#include "Components/RigidBody.h"

namespace DeltaEngine
{
  struct Point;
  void BuildLineSegment( LineSegment &lineSegment,
                         const  Vector2 &pos,
                         float scale,
                         float dir );
                   //COLLISION TESTS
bool CollisionIntersection_RectRect_Static( const Vector2 Center1, const Vector2 Size1, const Vector2 Center2, const Vector2 Size2 );

bool CollisionIntersection_RectRay_Static( const Vector2 Center1, const Vector2 Size1, const Vector2 Center2, const Vector2 Size2 );

bool CollisionIntersection_RectCircle( Collider &col1, RigidBody &r1, Collider &col2, RigidBody &r2, Manifold &m );

bool CollisionIntersection_RectLine_Static( const Vector2 Center1, const Vector2 Size1, const Vector2 Center2, const Vector2 Size2 );

bool CollisionIntersection_CircleCircle_Static( const Vector2 Center1, const Vector2 Size1, const Vector2 Center2, const Vector2 Size2 );

bool CollisionIntersection_CircleLineSegment_Static( const Vector2 Center1, const Vector2 Size1, const Vector2 Center2, const Vector2 Size2 );

bool CollisionIntersection_CircleLineSegment( Collider &col1, const Vector2 &vel1, Collider &col2, const Vector2 &vel2, Manifold &m );

bool CollisionIntersection_CircleLineSegment( Collider &col1, const Vector2 &vel1, LineSegment line, Manifold &m );

bool CheckMovingCircleToLineEdge( bool withinlines, const Circle &circle, const Vector2 &ptEnd, const LineSegment &lineSeg, Vector2 &interPt, Vector2 &normalAtCollision, float &interTime );

bool CollisionIntersection_RayLine_Static( const Vector2 Center1, const Vector2 Size1, const Vector2 Center2, const Vector2 Size2 );

bool CollisionIntersection_LineLine_Static( const Vector2 Center1, const Vector2 Size1, const Vector2 Center2, const Vector2 Size2 );

bool CollisionIntersection_RectPoint( const Vector2 Center1, const Vector2 Size1, const Vector2 Center2 );

bool CollisionIntersection_RectMouse( const Vector2 Center1, const Vector2 Size1, Point Center2 );

bool CollisionIntersection_CirclePoint( const Vector2 Center1, const Vector2 Size1, const Vector2 Center2 );

bool CollisionIntersection_CircleMouse( const Vector2 Center1, const Vector2 Size1, Point Center2 );

//Dynamic Collision Checks
bool CollisionIntersection_RectRect( const Collider &, const Vector2 &, const Collider &, const Vector2 & );

bool CollisionIntersection_CircleCircle( Collider &, const Vector2 &, const Collider &, const Vector2 &, Manifold &m );

bool CollisionIntersection_RayCircle( const Ray &ray, Collider &col2, Manifold &m );

//Main
bool CollisionIntersection_Main( Collider &, RigidBody &, Collider &, RigidBody &, Manifold &m );

bool CollisionIntersection_Sub_Box( Collider &, RigidBody &, Collider &, RigidBody &, Manifold &m );

bool CollisionIntersection_Sub_Circle( Collider &, RigidBody &, Collider &, RigidBody &, Manifold &m );

bool CollisionIntersection_Sub_Ray( Collider &, RigidBody &, Collider &, RigidBody &, Manifold &m );

bool CollisionIntersection_Sub_Line( Collider &, Collider &, Manifold &m );

//Calculate Manifold

bool AABBvsAABB_Manifold(Collider& A,Collider& B,Manifold &m);
//UNUSED FUNCTIONS

  //int CollisionIntersection_CircleLineSegment(const Circle& circle,
  //	const Vector2& ptEnd,
  //	const LineSegment& lineSeg,
  //	Vector2& interPt,
  //	Vector2& normalAtCollision,
  //	float& interTime,
  //	bool& checkLineEdges);

  //Need to add
  //int CheckMovingCircleToLineEdge(bool withinBothLines,
  //	const Circle& circle,
  //	const Vector2& ptEnd,
  //	const LineSegment& lineSeg,
  //	Vector2& interPt,
  //	Vector2& normalAtCollision,
  //	float& interTime);



}
