#pragma once

#include "DEpch.h"
#include "Vector.h"
#include <array>

namespace DeltaEngine
{
  struct BezierCurve
  {
    enum class Type
    {
      Constant,
      ConstantCurve,
      RandomBetweenConstants,
      RandomBetweenCurves,
    };

    // positions are saved as normalized vectors
    // position, active
    std::array<std::pair<Vector2, bool>, 8> anchors;
    // controls are local to anchors
    // position, active
    std::array<std::pair<Vector2, bool>, 8> controlsLeft;
    std::array<std::pair<Vector2, bool>, 8> controlsRight;

    // determines the true positions
    float min = 0.0f, max = 1.0f;

    BezierCurve();
    BezierCurve(int constant);

    // assume that x = time, this returns y
    float Evaluate(float time);
  };

  struct BezierRange
  {
    BezierCurve min, max;
    BezierCurve::Type type = BezierCurve::Type::Constant;
    bool minActive = true, maxActive = false;
    BezierRange() = default;
    BezierRange(int constant);
  };
  struct BezierRange3
  {
    BezierCurve minX, minY, minZ;
    BezierCurve maxX, maxY, maxZ;
    BezierCurve::Type type = BezierCurve::Type::Constant;
    bool minXActive = true, minYActive = true, minZActive = true;
    bool maxXActive = false, maxYActive = false, maxZActive = false;
    BezierRange3() = default;
    BezierRange3(int constant);
  };
}
