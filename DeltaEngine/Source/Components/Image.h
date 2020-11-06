#pragma once
#include "DEpch.h"
#include "Render/Sprite.h"

namespace DeltaEngine
{
  struct DE_API Image
  {
    Sprite m_Sprite;

    Vector2 m_Offset = Vector2(0, 0);
    Vector2 m_Tiling = Vector2(1, 1);

    bool m_FlipX = false;
    bool m_FlipY = false;
  };
}
