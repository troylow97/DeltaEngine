#pragma once

#include "DEpch.h"
#include "Vector.h"

namespace DeltaEngine
{
  struct BezierCurve
  {
    // positions are saved as normalized vectors
    // position
    std::vector<Vector2> anchors;
    // controls are local to anchors
    // position, active
    std::vector<std::pair<Vector2, bool>> controlsLeft;
    std::vector<std::pair<Vector2, bool>> controlsRight;

    // determines the true positions
    float min = 0.0f, max = 1.0f;

    BezierCurve();

    // assume that x = time, this returns y
    float Evaluate(float time);
  };
}
