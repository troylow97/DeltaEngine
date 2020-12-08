/**********************************************************************************
* \file   ImGuiMath.cpp
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
#include "ImGuiMath.h"

namespace DeltaEngine
{
  ImVec2 operator+(ImVec2 a, ImVec2 b)
  {
    return ImVec2(a.x + b.x, a.y + b.y);
  }

  ImVec2 operator+=(ImVec2& a, ImVec2 b)
  {
    a = a + b;
    return a;
  }

  ImVec2 operator-(ImVec2 a)
  {
    return ImVec2(-a.x, -a.y);
  }

  ImVec2 operator-(ImVec2 a, ImVec2 b)
  {
    return ImVec2(a.x - b.x, a.y - b.y);
  }

  ImVec2 operator-=(ImVec2& a, ImVec2 b)
  {
    a = a - b;
    return a;
  }

  ImVec2 operator*(ImVec2 a, float b)
  {
    return ImVec2(a.x * b, a.y * b);
  }

  ImVec2 operator*=(ImVec2& a, float b)
  {
    a = a * b;
    return a;
  }

  ImVec2 operator/(ImVec2 a, float b)
  {
    return ImVec2(a.x / b, a.y / b);
  }

  ImVec2 operator/=(ImVec2& a, float b)
  {
    a = a / b;
    return a;
  }
}
