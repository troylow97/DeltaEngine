#pragma once
#include <string>
#include "Core/Math/Vector.h"

struct AI
{
  DeltaEngine::Vector2 original_point;
  std::string key;
  std::string transition;

  AI() :
    original_point{{0,0}},
    key("idle"),
    transition("null")
  {
  }
};
