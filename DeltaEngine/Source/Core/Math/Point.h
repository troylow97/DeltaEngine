#pragma once

#include "DE_API.h"

namespace DeltaEngine
{

struct DE_API Point
{
  float point_x = 0;
  float point_y = 0;

  Point();
  Point( float x, float y );
  Point( const Point &point );
  bool operator== ( const Point &rhs );
  bool operator!= ( const Point &rhs );

};

}
