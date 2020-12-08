/**********************************************************************************
* \file   Sprite.h
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
#include "Core/Math/Vector.h"
#include "Texture.h"

namespace DeltaEngine
{
  class Sprite
  {
  public:
#pragma warning(disable:4251)
    std::string m_Key;
#pragma warning(default:4251)
    unsigned int m_Index;
    Sprite(std::string textureName = "", unsigned int index = 0);
    Sprite(const Sprite&);
    ~Sprite() = default;
    Sprite& operator=(const Sprite&);
    operator bool() const;
    unsigned int GetWidth() const;
    unsigned int GetHeight() const;
    Vector2 GetOffset() const;
    Vector2 GetTiling() const;
    Vector2 GetPivot() const;
    Texture2D* GetTexture() const;
    std::string GetName() const;
    std::string GetKey() const;
    unsigned int GetIndex() const;
  };
}
