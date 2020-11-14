#include "Point.h"

namespace DeltaEngine
{
  Point::Point() : Point(0, 0)
  {
  }

  Point::Point(float x, float y) :
    point_x(x),
    point_y(y)
  {
  }

  Point::Point(const Point& point) :
    point_x(point.point_x),
    point_y(point.point_y)
  {
  }

  bool Point::operator==(const Point& rhs)
  {
    if (point_x == rhs.point_x && point_y == rhs.point_y)
      return true;
    return false;
  }

  bool Point::operator!=(const Point& rhs)
  {
    return !(*this == rhs);
  }
}
