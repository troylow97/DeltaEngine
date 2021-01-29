/**********************************************************************************
* \file   Sprite.cpp
* \brief  The file contains implementation of Sprite class.
* \author Ong, Graeme,   100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "Sprite.h"
#include "Core/GlobalStruct.h"
#include "Assets/AssetManager.h"

namespace DeltaEngine
{
  Sprite::Sprite(std::string textureName, unsigned int index)
    : m_Key{textureName}, m_Index{index}
  {
  }

  Sprite::Sprite(const Sprite& copy)
    : m_Key{copy.m_Key.c_str()}, m_Index{copy.m_Index}
  {
  }

  Sprite& Sprite::operator=(const Sprite& rhs)
  {
    m_Key = string(rhs.m_Key);
    m_Index = rhs.m_Index;
    return *this;
  }

  Sprite::operator bool() const
  {
    return GetTexture() != nullptr;
  }

  unsigned int Sprite::GetWidth() const
  {
    if (*this)
      return static_cast<unsigned int>(GetTexture()->GetSize(m_Index).x);
    return 0;
  }

  unsigned int Sprite::GetHeight() const
  {
    if (*this)
      return static_cast<unsigned int>(GetTexture()->GetSize(m_Index).y);
    return 0;
  }

  Vector2 Sprite::GetTiling() const
  {
    if (*this)
      return Vector2(
        1.0f * GetWidth() / GetTexture()->GetWidth(),
        1.0f * GetHeight() / GetTexture()->GetHeight()
      );
    return Vector2(1.0f, 1.0f);
  }

  Vector2 Sprite::GetOffset() const
  {
    if (*this)
      return GetTexture()->GetOffset(m_Index);
    return Vector2(0.0f, 0.0f);
  }

  Vector2 Sprite::GetPivot() const
  {
    if (*this)
      return GetTexture()->GetPivot(m_Index);
    return Vector2(0.5f, 0.5f);
  }

  Texture2D* Sprite::GetTexture() const
  {
    if (m_Key.length() > 0)
      return GetEnv().pManager->Get<Texture2D>(native::to_string(m_Key));
    return nullptr;
  }

  Sprite::string Sprite::GetName() const
  {
    return m_Key + "_" + std::to_string(m_Index);
  }

  Sprite::string Sprite::GetKey() const
  {
    return m_Key;
  }

  unsigned int Sprite::GetIndex() const
  {
    return m_Index;
  }
}
