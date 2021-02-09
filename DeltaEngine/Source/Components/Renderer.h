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
#include "Image.h"
#include "Transform.h"
#include "Render/Material.h"
#include "Render/Window.h"
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

    Vector4 GetScreenspaceBounds(Image& i)
    {
      Transform t{};
      float refAspect = refRes.x / refRes.y;

      float tmpXscale = 1;
      float tmpYscale = 1;

      anchorMin.x = Math::Clamp01(anchorMin.x);
      anchorMin.y = Math::Clamp01(anchorMin.y);
      anchorMax.x = Math::Clamp01(anchorMax.x);
      anchorMax.y = Math::Clamp01(anchorMax.y);

      if (Math::Abs(anchorMax.x - anchorMin.x) > .01f)
      {
        float anch = (anchorMax.x - anchorMin.x) / 2 - .5f;
        float midpt = (left - right) / 2;
        t.position.x = anch + midpt;
        tmpXscale = (1 - (left + right));
        pivot = Vector2(.5f, .5f);
      }
      else
      {
        t.position.x = anchorMin.x;
        tmpXscale = size.x;
      }
      if (Math::Abs(anchorMax.y - anchorMin.y) > .01f)
      {
        float anch = (anchorMax.y - anchorMin.y) / 2 - .5f;
        float midpt = (bottom - top) / 2;
        t.position.y = anch + midpt;
        tmpYscale = (1 - (top + bottom));
        pivot = Vector2(.5f, .5f);
      }
      else
      {
        t.position.y = 1 - anchorMin.y;
        tmpYscale = size.y;
      }

      if (m_PreserveAspect)
      {
        float sprAspect = 1.0f * i.m_Sprite.GetWidth() / i.m_Sprite.GetHeight();

        if (tmpXscale / tmpYscale > sprAspect)
        {
          tmpXscale = tmpYscale * sprAspect / refAspect;
        }
        else
        {
          tmpYscale = tmpXscale / sprAspect * refAspect;
        }
      }

      return Vector4(
        (t.position.x * refRes.x - tmpXscale * pivot.x) / refRes.x,
        (t.position.y * refRes.y - tmpYscale * (1 - pivot.y)) / refRes.y,
        (t.position.x * refRes.x + tmpXscale * (1 - pivot.x)) / refRes.x,
        (t.position.y * refRes.y + tmpYscale * pivot.y) / refRes.y);
    }
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
