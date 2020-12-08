/**********************************************************************************
* \file   Font.h
* \brief  The file contains interface of Font class.
* \author Ong, Graeme,   100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "DEpch.h"
#include "Core/Math/Vector.h"

namespace DeltaEngine
{
  struct CharacterInfo
  {
    unsigned int textureID = 0;
    Vector2 size = Vector2();
    Vector2 bearing = Vector2();
    unsigned int advance = 0;
  };

  class Font
  {
    std::unordered_map<char, CharacterInfo> m_CharacterInfo;
    unsigned int m_RendererID;
  public:
    Font(const std::string& filepath);
    std::unordered_map<char, CharacterInfo>& characterInfo();
    unsigned int GetID() const;
    static void Init();
    static void Exit();
  };
}
