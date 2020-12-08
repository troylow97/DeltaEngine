/**********************************************************************************
* \file   Collisions.h
* \brief  The file contains the functions needed for CollisionSystem
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "Physics/CollisionShapes.h"
#include "Manifold.h"
#include "Components/RigidBody.h"

namespace DeltaEngine
{
  struct Point;
  void BuildLineSegment(LineSegment& lineSegment,
                        const Vector2& pos,
                        float scale,
                        float dir);
  //COLLISION TESTS
  bool CollisionIntersection_RectRect_Static(Vector2 Center1, Vector2 Size1, Vector2 Center2, Vector2 Size2);

  bool CollisionIntersection_RectRay_Static(Vector2 Center1, Vector2 Size1, Vector2 Center2, Vector2 Size2);

  bool CollisionIntersection_RectCircle(Collider& col1, RigidBody& r1, Collider& col2, RigidBody& r2, Manifold& m);

  bool CollisionIntersection_RectLine_Static(Vector2 Center1, Vector2 Size1, Vector2 Center2, Vector2 Size2);

  bool CollisionIntersection_CircleCircle_Static(Vector2 Center1, Vector2 Size1, Vector2 Center2, Vector2 Size2,
                                                 Manifold& m);

  bool CollisionIntersection_CircleLineSegment_Static(Vector2 Center1, Vector2 Size1, Vector2 Center2, Vector2 Size2);

  bool CollisionIntersection_CircleLineSegment(Collider& col1, const Vector2& vel1, Collider& col2, const Vector2& vel2,
                                               Manifold& m);

  bool CollisionIntersection_CircleLineSegment(Collider& col1, const Vector2& vel1, LineSegment line, Manifold& m);

  bool CheckMovingCircleToLineEdge(bool withinlines, const Circle& circle, const Vector2& ptEnd,
                                   const LineSegment& lineSeg, Vector2& interPt, Vector2& normalAtCollision,
                                   float& interTime);

  bool CollisionIntersection_RayLine_Static(Vector2 Center1, Vector2 Size1, Vector2 Center2, Vector2 Size2);

  bool CollisionIntersection_LineLine_Static(Vector2 Center1, Vector2 Size1, Vector2 Center2, Vector2 Size2);

  bool CollisionIntersection_RectPoint(Vector2 Center1, Vector2 Size1, Vector2 Center2);

  bool CollisionIntersection_RectMouse(Vector3 Center1, Vector2 Size1, Point Center2);

  bool CollisionIntersection_CirclePoint(Vector2 Center1, Vector2 Size1, Vector2 Center2);

  //bool CollisionIntersection_CircleMouse(Vector2 Center1, Vector2 Size1, Point Center2);

  //Dynamic Collision Checks
  bool CollisionIntersection_RectRect(const Vector2&,const Vector2&, const Vector2&, const Vector2&, const Vector2&, const Vector2&);

  bool CollisionIntersection_CircleCircle(Collider&, const Vector2&, const Collider&, const Vector2&, Manifold& m);

  bool CollisionIntersection_RayCircle(const Ray& ray, Collider& col2, Manifold& m);

  //Calculate Manifold

  bool AABBvsAABB_Manifold(Collider& A, Vector2 scaleA, Collider& B, Vector2 scaleB, Manifold& m);
}
