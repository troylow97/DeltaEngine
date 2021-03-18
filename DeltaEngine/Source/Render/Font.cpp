/**********************************************************************************
* \file   Font.cpp
* \brief  The file contains implementation of Font class.
* \author Ong, Graeme,   100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "Font.h"
#include "Core/Debugging/Logger/Log.h"
#include "ErrorCheck.h"
#include <GL/glew.h>
#include <ft2build.h>
#include FT_FREETYPE_H


#include "crtdbg.h"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

namespace DeltaEngine
{
  FT_Library ft;

  Font::Font(const std::string& filepath) : m_RendererID{0}
  {
    DeltaEngine_CORE_TRACE("Loading font {}...", filepath);
    FT_Face face;
    if (FT_New_Face(ft, filepath.c_str(), 0, &face))
    {
      DeltaEngine_CORE_ERROR("FREETYTPE: Failed to load {0} font!", filepath);
      return;
    }
    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);

    GLCall(glPixelStorei( GL_UNPACK_ALIGNMENT, 1 )); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
      // load character glyph 
      if (FT_Load_Char(face, c, FT_LOAD_RENDER))
      {
        DeltaEngine_CORE_ERROR("FREETYTPE: Failed to load Glyph: {0} ", c);
        continue;
      }
      // generate texture
      glGenTextures(1, &m_RendererID);
      glBindTexture(GL_TEXTURE_2D, m_RendererID);
      glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer
      );
      // set texture options
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      // now store character for later use
      CharacterInfo character = {
        m_RendererID,
        Vector2(static_cast<float>(face->glyph->bitmap.width), static_cast<float>(face->glyph->bitmap.rows)),
        Vector2(static_cast<float>(face->glyph->bitmap_left), static_cast<float>(face->glyph->bitmap_top)),
        static_cast<unsigned>(face->glyph->advance.x)
      };
      m_CharacterInfo.insert(std::pair<char, CharacterInfo>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    DeltaEngine_CORE_TRACE("Font {} was loaded successfully", filepath);
  }

  std::unordered_map<char, CharacterInfo>& Font::characterInfo()
  {
    return m_CharacterInfo;
  }

  unsigned Font::GetID() const
  {
    return m_RendererID;
  }

  void Font::Init()
  {
    DeltaEngine_CORE_INFO("Initializing FreeType...");
    if (FT_Init_FreeType(&ft))
    {
      DeltaEngine_CORE_ERROR("Failed to initialize FreeType Library!");
      return;
    }
    DeltaEngine_CORE_INFO("Initializing FreeType successful");
  }

  void Font::Exit()
  {
    FT_Done_FreeType(ft);
  }
}
