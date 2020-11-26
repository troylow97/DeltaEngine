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
    Repeat, Mirror, Clamp
  };

  class Texture2D
  {
    unsigned int m_RendererID;
    int m_Channels;
    int m_Width, m_Height;
    std::string m_Filepath, m_Name;
  public:
    Texture2D(std::string filepath = "dog.png");
    ~Texture2D();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    int GetWidth() const;
    int GetHeight() const;

    unsigned int GetRendererID() const;

    void AutoSlice(Vector2 pivot = Vector2(0.5f, 0.5f), bool noOverlap = true);
    void Slice(TextureInfo info);
    void SliceAll(unsigned int columns, unsigned int rows, Vector2 pivot = Vector2(0.5f, 0.5f));

    Vector2 GetOffset(unsigned int index);
    Vector2 GetSize(unsigned int index);
    Vector2 GetPivot(unsigned int index);

    std::string GetName();
    std::vector<TextureInfo> textureInfo;

    TextureWrapMode wrapMode;
  private:
    void InitTexture(std::string filepath);
    void LoadMetaFile(std::string filepath);
    void UpdateMetaFile(std::string filepath);
  };
}
