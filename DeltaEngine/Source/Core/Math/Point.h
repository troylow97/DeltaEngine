/**********************************************************************************
* \file   Point.h
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

namespace DeltaEngine
{
  struct Point
  {
    float point_x = 0;
    float point_y = 0;

    Point();
    Point(float x, float y);
    Point(const Point& point);
    Point operator+ ( const Point &point );
    Point operator- ( const Point &point );

    bool operator==(const Point& rhs);
    bool operator!=(const Point& rhs);
  };
}
