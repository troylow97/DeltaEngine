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

    Vector2 m_Offset;
    Vector2 m_Tiling;

    bool m_FlipX;
    bool m_FlipY;

    FillType m_FillType;
    float m_FillAmount;
    float m_OverallAngle;
    float m_StartAngle, m_EndAngle;

    Image()
      : m_Sprite{ "" },
      m_Offset{ 0, 0 },
      m_Tiling{ 1, 1 },
      m_FlipX{ false },
      m_FlipY{ false },
      m_FillType{ FillType::None },
      m_FillAmount{ 1 },
      m_OverallAngle{ 0 },
      m_StartAngle{ 0 },
      m_EndAngle{ 0 } {}

    Image& operator= (const Image& rhs)
    {
      m_Sprite = rhs.m_Sprite;
      m_Offset = rhs.m_Offset;
      m_Tiling = rhs.m_Tiling;
      m_FlipX = rhs.m_FlipX;
      m_FlipY = rhs.m_FlipY;
      m_FillType = rhs.m_FillType;
      m_FillAmount = rhs.m_FillAmount;
      m_OverallAngle = rhs.m_OverallAngle;
      m_StartAngle = rhs.m_StartAngle;
      m_EndAngle = rhs.m_EndAngle;
      return *this;
    }
  };
}