#pragma once
#include "DEpch.h"
#include "Render/Sprite.h"

namespace DeltaEngine
{
  enum class FillType
  {
    None,
    HorizontalLeftToRight,
    HorizontalRightToLeft,
    VerticalTopToBottom,
    VerticalBottomToTop,
    Radial360Clockwise,
    Radial360AntiClockwise,
  };

  struct Image
  {
    Sprite m_Sprite;

    Vector2 m_Size;
    Vector2 m_Offset;
    Vector2 m_Tiling;

    FillType m_FillType;
    float m_FillAmount;
    float m_OverallAngle;
    float m_StartAngle, m_EndAngle;
    bool m_FlipX;
    bool m_FlipY;

    Image()
      : m_Sprite{""},
        m_Size{1, 1},
        m_Offset{0, 0},
        m_Tiling{1, 1},
        m_FillType{FillType::None},
        m_FillAmount{1},
        m_OverallAngle{0},
        m_StartAngle{0},
        m_EndAngle{0},
        m_FlipX{false},
        m_FlipY{false}
    {
    }

    Vector2 GetWorldSize()
    {
      return Vector2
      (
        m_Sprite.GetWidth() / 200.0f * m_Size.x,
        m_Sprite.GetHeight() / 200.0f * m_Size.y
      );
    }
  };
}
