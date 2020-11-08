#pragma once
#include "DEpch.h"
#include "Render/Material.h"
#include "Core/Math/Color.h"

namespace DeltaEngine
{
  struct Renderer2D
  {
    Material m_Material;
    Color color;
    bool m_Active = true;

    bool m_Shaded = true;
    bool m_Wireframe = true;

    Renderer2D(): m_Material(), color(), m_Active{true}, m_Shaded{true}, m_Wireframe{true} {};
  };
}
