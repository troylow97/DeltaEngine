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
      m_Wireframe{true}
    {
    };
  };
}
