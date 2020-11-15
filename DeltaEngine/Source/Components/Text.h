#pragma once
#include "DEpch.h"
#include "Render/Font.h"
#include "Core/Math/Color.h"

namespace DeltaEngine
{
  enum class Alignment : unsigned
  {
    AlignLeft,
    AlignRight,
    Centralize,
  };

  struct Text
  {
    std::string m_FontKey = "Default";

    std::string m_Text = "";

    Alignment alignment = Alignment::AlignLeft;

    Text() = default;
  };
}
