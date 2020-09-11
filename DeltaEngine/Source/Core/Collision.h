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

#include <SFML/Window.hpp>

/**************************************************************************/
/*!
    a struct for bounding boxes
*/
/**************************************************************************/
template <typename T>
struct AABB
{
    sf::Vector2<T> min;
    sf::Vector2<T> max;
};

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
                                    const AABB<T>& aabb2, const sf::Vector2<T>& vel2);
