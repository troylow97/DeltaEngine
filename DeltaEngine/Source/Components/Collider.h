/**********************************************************************************
* \file   Collider.h
* \brief  The file contains BLAHBLAHBLAH
* \author Chin, Clara,   X% Code Contribution
* \author Low, Troy,     X% Code Contribution
* \author Ong, Graeme,   X% Code Contribution
* \author Tan, Tong Wee, X% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "Core/Math/Vector.h"

namespace DeltaEngine
{
enum class ColliderType : unsigned
{
  NONE,
  BOX,
  CIRCLE,
  LINE,
  RAY
};

struct Collider
{
  Vector2 center;
  Vector2 offset;
  Vector2 size;
  Vector2 interPoint;
  ColliderType type;
  unsigned CollisionLayerCheck;
  unsigned CollisionLayerID;
  bool isTrigger;
  bool isCollidingOnFloor;
  bool isPlatform;

  Collider() :
    center { 0, 0 },
    offset { 0, 0 },
    size { 1, 1 },
    interPoint { 0, 0 },
    type { ColliderType::BOX },
    CollisionLayerCheck { 255 },
    CollisionLayerID { 255 },
    isTrigger { false },
    isCollidingOnFloor { false },
	isPlatform{false}
  {
  }

  Collider( Vector2 new_center, Vector2 new_size, ColliderType col_type ) :
    center( new_center ),
    offset { 0, 0 },
    size( new_size ),
    interPoint { 0, 0 },
    type { ColliderType::BOX },
    CollisionLayerCheck { 255 },
    CollisionLayerID { 255 },
    isTrigger { false },
    isCollidingOnFloor { false },
    isPlatform{ false }
  {
    (void) col_type;
  }
};

//0001 Wall
//0010 Player
//0100 Enemy
//1000 Bullet
} // namespace DeltaEngine
