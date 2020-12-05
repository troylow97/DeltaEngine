#pragma once
#include "DEpch.h"
#include "Render/Font.h"
#include "Core/Math/Color.h"

namespace DeltaEngine
{
  enum class UIType : unsigned
  {
    Screen,
    Interface,
    Button
  };

  struct UI
  {
    std::string functor_key;
    UIType ui_type;
    unsigned screen;
    int target_screen{-1};
    int previous_screen{-1};
    bool overlay { false };
  };
}
