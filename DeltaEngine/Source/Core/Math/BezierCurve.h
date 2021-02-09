#pragma once

#include "DEpch.h"
#include "Vector.h"

namespace DeltaEngine
{
  struct BezierCurve
  {
    // positions are saved as normalized vectors
    // position, active
    //std::array<std::pair<Vector2, bool>, 8> anchors;
    // controls are local to anchors
    // position, active
    //std::array<std::pair<Vector2, bool>, 8> controlsLeft;
    //std::array<std::pair<Vector2, bool>, 8> controlsRight;

    // determines the true positions
    float min = 0.0f, max = 1.0f;

    BezierCurve();
    BezierCurve(int constant);

    // assume that x = time, this returns y
    float Evaluate(float time);
  };
}
