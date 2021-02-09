#pragma once

#include "DEpch.h"
#include "Color.h"

namespace DeltaEngine
{
  struct Gradient
  {
    // value, location (0 - 1)
    // alpha is active
    //std::array<std::pair<Color, float>, 8> colorKeys;
    // negative is inactive
    //std::array<std::pair<float, float>, 8> alphaKeys;

    Gradient();

    // time is location
    Color Evaluate(float time);
  };
}
