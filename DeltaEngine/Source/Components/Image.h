#pragma once
#include "DEpch.h"
#include "Render/Sprite.h"

namespace DeltaEngine
{
  struct Image
  {
    Sprite m_Sprite;

    Vector2 m_Offset;
    Vector2 m_Tiling;

    bool m_FlipX;
    bool m_FlipY;

    Image()
      : m_Sprite{ "" },
      m_Offset{ 0, 0 },
      m_Tiling{ 1, 1 },
      m_FlipX{ false },
      m_FlipY{ false } {}
    bool m_FlipX = false;
    bool m_FlipY = false;

    Image& operator= (const Image& rhs)
    {
      m_Sprite = rhs.m_Sprite;
      m_Offset = rhs.m_Offset;
      m_Tiling = rhs.m_Tiling;
      m_FlipX = rhs.m_FlipX;
      m_FlipY = rhs.m_FlipY;
      return *this;
    }
  };
}