/**********************************************************************************
* \file   Point.cpp
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

  Point Point::operator+(const Point& point)
  {
    return Point( point_x + point.point_x, point_y + point.point_y );
  }

  Point Point::operator-(const Point& point)
  {
    return Point( point_x - point.point_x, point_y - point.point_y );
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
