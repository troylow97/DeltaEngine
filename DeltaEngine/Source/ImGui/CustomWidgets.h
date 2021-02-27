#pragma once

#include "Core/Math/BezierCurve.h"
#include "Core/Math/Gradient.h"

namespace DeltaEngine
{
  bool BezierEdit(const char* label, BezierRange* bezier);
  bool BezierEdit3(const char* label, BezierRange3* bezier);
  bool GradientEdit(const char* label, GradientRange* gradient);
}
