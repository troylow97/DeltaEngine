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
    Sprite();
    Sprite(std::string textureName = "", unsigned int index = 0);
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
