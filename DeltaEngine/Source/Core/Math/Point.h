#pragma once

#include "DE_API.h"

namespace DeltaEngine
{

struct DE_API Point
{
  int point_x = 0;
  int point_y = 0;

  Point();
  Point( int x, int y );
  Point( const Point &point );
  bool operator== ( const Point &rhs );
  bool operator!= ( const Point &rhs );

};

}
