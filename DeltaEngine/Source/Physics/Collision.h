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

namespace DeltaEngine
{
    /**************************************************************************/
    /*!
        a struct for bounding boxes
    */
    /**************************************************************************/
    struct DE_API AABB
    {
        Vector2 min;
        Vector2 max;
    };

	struct LineSegment
	{
		Vector2	m_pt0;
		Vector2	m_pt1;
		Vector2	m_normal; //outward
	};

	struct Circle
	{
		Vector2  m_center;
		float	m_radius;
		float   m_mass{ 1.0f };
	};

	struct Ray
	{
		Vector2	m_pt0;
		Vector2	m_dir;
	};

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

	void CollisionResponse_CircleLineSegment(const Vector2& ptInter,
		const Vector2& normal,
		Vector2& ptEnd,															
		Vector2& reflected);													

	void CollisionResponse_CirclePillar(const Vector2& normal,
		const float& interTime,														
		const Vector2& ptStart,													
		const Vector2& ptInter,													
		Vector2& ptEnd,															
		Vector2& reflectedVectorNormalized);									

	/******************************************************************************/
	/*!
		Calculates reflection when the circle hits another circle.
		A reflection vector is calculated and the end point is also updated.

		This calculation takes into account the mass of two circles.
	 */
	 /******************************************************************************/
	void CollisionResponse_CircleCircle(Vector2& normal,						//Normal vector of reflection on collision time - input
		const float interTime,														//Intersection time - input
		Vector2& velA,															//Velocity of CircleA - input
		const float& massA,															//Mass of CircleA - input
		Vector2& interPtA,														//Intersection position of circle A at collision time - input
		Vector2& velB,															//Velocity of CircleB - input
		const float& massB,															//Mass of CircleB - input
		Vector2& interPtB,														//Intersection position of circle B at collision time - input
		Vector2& reflectedVectorA,												//Non-Normalized reflected vector of Circle A - output
		Vector2& ptEndA,														//Final position of the circle A after reflection - output
		Vector2& reflectedVectorB,												//Non-Normalized reflected vector of Circle B - output
		Vector2& ptEndB);														//Final position of the circle B after reflection - output

}
