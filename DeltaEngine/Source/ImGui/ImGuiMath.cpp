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
