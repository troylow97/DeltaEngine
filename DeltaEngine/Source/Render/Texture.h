/**********************************************************************************
* \file   Texture.h
* \brief  The file contains interface of Sprite class.
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
  struct TextureInfo
  {
    Vector2 offset;
    Vector2 size;
    Vector2 pivot;
  };

  enum class TextureWrapMode
  {
    Repeat,
    Mirror,
    Clamp
  };

  class Texture2D
  {
    unsigned m_RendererID;
    int m_Channels;
    int m_Width, m_Height;
    std::string m_Filepath, m_Name;
  public:
    Texture2D(std::string filepath = "dog.png");
    ~Texture2D();

    static void BindID(unsigned id, unsigned slot = 0);
    void Bind(unsigned slot = 0) const;
    static void Unbind();

    int GetWidth() const;
    int GetHeight() const;

    unsigned GetRendererID() const;

    void Slice(std::vector<TextureInfo>);
    std::vector<TextureInfo> AutoSlice(Vector2 pivot = Vector2(0.5f, 0.5f), bool noOverlap = true);
    std::vector<TextureInfo> SliceAll(unsigned columns, unsigned rows, Vector2 pivot = Vector2(0.5f, 0.5f));

    Vector2 GetOffset(unsigned index);
    Vector2 GetSize(unsigned index);
    Vector2 GetPivot(unsigned index);

    std::string GetName();
    std::string GetFilepath();
    std::vector<TextureInfo> textureInfo;

    TextureWrapMode wrapMode;

    void UpdateWrapMode(int mode);
    void InitTexture();
    void LoadInfoFile();
    void UpdateInfoFile();
  };
}
