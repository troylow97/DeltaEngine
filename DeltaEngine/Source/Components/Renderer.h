#pragma once
#include "DEpch.h"
#include "Render/Material.h"
#include "Core/Math/Color.h"

namespace DeltaEngine
{
  struct DE_API Renderer2D
  {
    Material material;
    Color color;
    bool m_Active = true;

    bool m_Shaded = true;
    bool m_Wireframe = true;
  };
}
