/* Start Header******************************************************************/
/*!
\file		Collision.cpp
\author 	CHIN Mei Qi, Clara, m.chin, 440004919
\par    	email: m.chin\@digipen.edu
\date   	March 11, 2020
\brief      This file contains the functions for the collision

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /* End Header *******************************************************************/

#include "Core/Collision.h"

/**************************************************************************/
/*!
  \brief    check the collision for 2 rectangles

  \param    aabb1  min and max for the 1st object
  \param    vel1   velocity for the 1st object
  \param    aabb2  min and max for the 2nd object
  \param    vel2   velocity for the 2nd object
*/
/**************************************************************************/
template <typename T>
bool CollisionIntersection_RectRect(const AABB<T>& aabb1, const sf::Vector2<T>& vel1,
	                                const AABB<T>& aabb2, const sf::Vector2<T>& vel2)
{
	// checking for static collision
	if (((aabb1.min.x < aabb2.max.x) && (aabb1.min.y < aabb2.max.y))
		&& ((aabb2.min.x < aabb1.max.x) && (aabb2.min.y < aabb1.max.y)))
	{
		return true;
	}
	// tFirst, tLast
	f32 tFirst = 0.0f;
	f32 tLast = g_dt;

	// dFirst, dLast
	f32 dFirst = 0.0f;
	f32 dLast = 0.0f;

	// getting relative velocity by keeping the velocity of 1 object constant
	AEVec2 relativeVelocity;
	relativeVelocity.x = vel1.x - vel2.x;
	relativeVelocity.y = vel1.y - vel2.y;

	if (relativeVelocity.x == 0.0f)
	{
		if ((aabb2.min.x > aabb1.max.x) || (aabb1.min.x > aabb2.max.x))
		{
			return false;
		}
	}

	if (relativeVelocity.x < 0.0f)
	{
		// checking for case 1
		if (aabb1.min.x > aabb2.max.x)
		{
			return false;
		}
		// checking for case 4
		if (aabb1.max.x < aabb2.min.x)
		{
			// the shortest distance they need to traverse in order to be in contact
			dFirst = aabb1.max.x - aabb2.min.x;
			// calculate new tFirst
			f32 temp_tFirst = dFirst / relativeVelocity.x;
			// comparing the different tFirst and choosing the bigger one
			tFirst = max(temp_tFirst, tFirst);
		}
		if (aabb1.min.x < aabb2.max.x)
		{
			// the longest distance they need to traverse in order to be NOT in contact
			dLast = aabb1.min.x - aabb2.max.x;
			// calculate new tLast
			f32 temp_tLast = dLast / relativeVelocity.x;
			// comparing the different tLast and choosing the smaller one
			tLast = min(temp_tLast, tLast);
		}
	}

	if (relativeVelocity.x > 0.0f)
	{
		// checking for case 2
		if (aabb1.min.x > aabb2.max.x)
		{
			// the shortest distance they need to traverse in order to be in contact
			dFirst = aabb1.min.x - aabb2.max.x;
			// calculate new tFirst
			f32 temp_tFirst = dFirst / relativeVelocity.x;
			// comparing the different tFirst and choosing the bigger one
			tFirst = max(temp_tFirst, tFirst);
		}
		if (aabb1.max.x > aabb2.min.x)
		{
			// the longest distance they need to traverse in order to be NOT in contact
			dLast = aabb1.max.x - aabb2.min.x;
			// calculate new tLast
			f32 temp_tLast = dLast / relativeVelocity.x;
			// comparing the different tLast and choosing the smaller one
			tLast = min(temp_tLast, tLast);
		}
		// checking for case 3
		if (aabb1.max.x < aabb2.min.x)
		{
			return false;
		}
	}

	// checking for case 5
	if (tFirst > tLast)
	{
		return false;
	}

	if (relativeVelocity.y == 0.0f)
	{
		if ((aabb2.min.y > aabb1.max.y) || (aabb1.min.y > aabb2.max.y))
		{
			return false;
		}
	}

	if (relativeVelocity.y < 0.0f)
	{
		// checking for case 1
		if (aabb1.min.y > aabb2.max.y)
		{
			return false;
		}
		// checking for case 4
		if (aabb1.max.y < aabb2.min.y)
		{
			// the shortest distance they need to traverse in order to be in contact
			dFirst = aabb1.max.y - aabb2.min.y;
			// calculate new tFirst
			f32 temp_tFirst = dFirst / relativeVelocity.y;
			// comparing the different tFirst and choosing the bigger one
			tFirst = max(temp_tFirst, tFirst);
		}
		if (aabb1.min.y < aabb2.max.y)
		{
			// the longest distance they need to traverse in order to be NOT in contact
			dLast = aabb1.min.y - aabb2.max.y;
			// calculate new tLast
			f32 temp_tLast = dLast / relativeVelocity.y;
			// comparing the different tLast and choosing the smaller one
			tLast = min(temp_tLast, tLast);
		}
	}

	if (relativeVelocity.y > 0.0f)
	{
		// checking for case 2
		if (aabb1.min.y > aabb2.max.y)
		{
			// the shortest distance they need to traverse in order to be in contact
			dFirst = aabb1.min.y - aabb2.max.y;
			// calculate new tFirst
			f32 temp_tFirst = dFirst / relativeVelocity.y;
			// comparing the different tFirst and choosing the bigger one
			tFirst = max(temp_tFirst, tFirst);
		}
		if (aabb1.max.y > aabb2.min.y)
		{
			// the longest distance they need to traverse in order to be NOT in contact
			dLast = aabb1.max.y - aabb2.min.y;
			// calculate new tLast
			f32 temp_tLast = dLast / relativeVelocity.y;
			// comparing the different tLast and choosing the smaller one
			tLast = min(temp_tLast, tLast);
		}
		// checking for case 3
		if (aabb1.max.y < aabb2.min.y)
		{
			return false;
		}
	}

	// checking for case 5
	if (tFirst > tLast)
	{
		return false;
	}
	return true;
}