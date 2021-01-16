/**********************************************************************************
* \file   Renderer.h
* \brief  The file contains BLAHBLAHBLAH
* \author Chin, Clara,   X% Code Contribution
* \author Low, Troy,     X% Code Contribution
* \author Ong, Graeme,   X% Code Contribution
* \author Tan, Tong Wee, X% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "DEpch.h"
#include "Render/Material.h"
#include "Core/Math/Color.h"
#include "Core/Debugging/Logger/Log.h"

namespace DeltaEngine
{
  struct RendererOverlay
  {
    Vector2 refRes;
    // no stretch
    // this is more useful for guaranteeing objects remaining in a specific displacement from the anchor point
    // while guaranteeing width/height ratio is preserved without preserved aspect
    Vector2 pos, size;
    // stretch
    // this is more useful for images that you want guaranteed to stretch across the screen, such as faders
    // may not have desirable behaviour with preserved aspect
    float left, right, top, bottom;

    // special overlay transform
    Vector2 anchorMin, anchorMax;
    Vector2 pivot;

    // default renderer stuff
    Material m_Material;
    Color m_Color;

    int m_SortingLayer;
    float m_SortingOrder;

    bool m_PreserveAspect = false; // width and height will be considered for final rendering
    bool m_Active = true;
    bool m_Shaded = true;
    bool m_Wireframe = false;

    RendererOverlay() :
      refRes(Vector2(1920, 1080)),
      pos(Vector2::zero()),
      size(Vector2::one() * 100.0f),
      left(0),
      right(0),
      top(0),
      bottom(0),
      anchorMin(Vector2::one() * 0.5f),
      anchorMax(Vector2::one() * 0.5f),
      pivot(Vector2::one() * 0.5f),
      m_Material(),
      m_Color(),
      m_SortingLayer{ 0 },
      m_SortingOrder{ 0.0f },
      m_Active{ true },
      m_Shaded{ true },
      m_Wireframe{ false }
    {
    };
  };

  struct Renderer2D
  {
    Material m_Material;
    Color m_Color;

    int m_SortingLayer;
    float m_SortingOrder;

    bool m_Active = true;
    bool m_Shaded = true;
    bool m_Wireframe = false;

    Renderer2D() :
      m_Material(),
      m_Color(),
      m_SortingLayer{0},
      m_SortingOrder{0.0f},
      m_Active{true},
      m_Shaded{true},
      m_Wireframe{false}
    {
    };
  };
}
