#pragma once
#include "DEpch.h"
#include "Render/Material.h"
#include "Core/Math/Color.h"
#include "Core/Debugging/Logger/Log.h"

namespace DeltaEngine
{
  struct Renderer2D
  {
    Material m_Material;
    Color m_Color;

    int m_SortingLayer;
    float m_SortingOrder;

    bool m_Active = true;
    bool m_Shaded = true;
    bool m_Wireframe = true;

    Renderer2D() : m_Material(), m_Color(), m_SortingLayer{0}, m_SortingOrder{0.0f}, m_Active{true}, m_Shaded{true},
                   m_Wireframe{true}
    {
    };
  };
}
