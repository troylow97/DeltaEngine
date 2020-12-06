#pragma once

#include "imgui.h"

namespace DeltaEngine
{
  ImVec2 operator+(ImVec2 a, ImVec2 b);
  ImVec2 operator+=(ImVec2& a, ImVec2 b);
  ImVec2 operator-(ImVec2 a);
  ImVec2 operator-(ImVec2 a, ImVec2 b);
  ImVec2 operator-=(ImVec2& a, ImVec2 b);
  ImVec2 operator*(ImVec2 a, float b);
  ImVec2 operator*=(ImVec2& a, float b);
  ImVec2 operator/(ImVec2 a, float b);
  ImVec2 operator/=(ImVec2& a, float b);
}
