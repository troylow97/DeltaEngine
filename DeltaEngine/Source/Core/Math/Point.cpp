#include "Point.h"

namespace DeltaEngine
{

Point::Point() : Point( 0, 0 )
{}

Point::Point( int x, int y ) :
  point_x( x ),
  point_y( y )
{}

Point::Point( const Point &point ) :
  point_x( point.point_x ),
  point_y( point.point_y )
{}

}