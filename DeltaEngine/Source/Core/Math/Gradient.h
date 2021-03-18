#pragma once

#include "DEpch.h"
#include "Vector.h"
#include "Color.h"

namespace DeltaEngine
{
  struct Gradient
  {
    enum class Type : unsigned
    {
      ConstantColor,
      ConstantGradient,
      RandomBetweenColors,
      RandomBetweenGradients,
    };

    // value, location (0 - 1)
    // alpha is active
    std::array<Color, 4> colorKeys;
    // negative is inactive
    std::array<Vector2, 4> alphaKeys;

    Gradient();

    // time is location
    Color Evaluate(float time);
  };

  struct GradientRange
  {
    Gradient min, max;
    Gradient::Type type = Gradient::Type::ConstantGradient;
  };
}
