#pragma once
#include "DEpch.h"
#include "Render/Material.h"
#include "Core/Math/Color.h"

namespace DeltaEngine
{
  struct DE_API Renderer2D
  {
    Material m_Material;
    Color m_Color;
    bool m_Active = true;

    int m_SortingLayer;
    float m_SortingOrder;

    bool m_Shaded = true;
    bool m_Wireframe = true;
  };
}
