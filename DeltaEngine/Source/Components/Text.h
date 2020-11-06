#pragma once
#include "DEpch.h"
#include "Render/Font.h"
#include "Core/Math/Color.h"

namespace DeltaEngine
{
  struct DE_API Text
  {
    std::string m_FontKey = "Default";

    std::string m_Text = "";

    enum class Alignment
    {
      AlignLeft,
      AlignRight,
      Centralize,
    };

    Alignment alignment = Alignment::AlignLeft;
  };
}
