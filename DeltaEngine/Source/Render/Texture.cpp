#include "Texture.h"
#include "ErrorCheck.h"
#include "Core/Debugging/Logger/Log.h"
#include <stb_image.h>
#include "DEpch.h"
#include <stack>

namespace DeltaEngine
{
  Texture2D::Texture2D(std::string filepath) :
    m_RendererID{ 0 },
    m_Channels{ 0 },
    m_Width{ 0 },
    m_Height{ 0 },
    m_Filepath{ filepath },
    m_Name{ filepath },
    wrapMode{ TextureWrapMode::Repeat }
  {
    InitTexture();
  }

  Texture2D::~Texture2D()
  {
    DeltaEngine_CORE_INFO("Deleting Texture {}", m_Name);
    GLCall(glDeleteTextures( 1, &m_RendererID ));
    DeltaEngine_CORE_INFO("Texture deleted");
  }

  void Texture2D::Bind(unsigned int slot) const
  {
    glActiveTexture(GL_TEXTURE0 + slot);
    GLCall(glBindTexture( GL_TEXTURE_2D, m_RendererID ));
  }

  void Texture2D::Unbind() const
  {
    GLCall(glBindTexture( GL_TEXTURE_2D, 0 ));
  }

  int Texture2D::GetWidth() const
  {
    return m_Width;
  }

  int Texture2D::GetHeight() const
  {
    return m_Height;
  }

  unsigned int Texture2D::GetRendererID() const
  {
    return m_RendererID;
  }

  void Texture2D::AutoSlice(Vector2 pivot, bool noOverlap)
  {
    textureInfo.clear();
    unsigned char* m_Data = stbi_load(m_Filepath.c_str(), &m_Width, &m_Height, &m_Channels, 0);

    DeltaEngine_CORE_INFO("Auto slicing texture \"{}\"", m_Filepath);

    // only auto slice if there is an alpha channel
    if (m_Channels != 4)
    {
      textureInfo.push_back({
        Vector2(0, 0), Vector2(static_cast<float>(m_Width), static_cast<float>(m_Height)), Vector2(0.5f, 0.5f)
      });
    }
    else
    {
      std::stack<std::pair<unsigned int, unsigned int>> stack;
      for (unsigned int x = 0; x < static_cast<unsigned>(m_Width); ++x)
        for (unsigned int y = 0; y < static_cast<unsigned>(m_Height); ++y)
        {
          // check for a non-transparent pixel
          if (*(m_Data + (static_cast<long long>(x) + static_cast<long long>(y) * m_Width) * 4 + 3) != 0)
          {
            unsigned int minX = m_Width, maxX = 0, minY = m_Height, maxY = 0;
            unsigned int a = x, b = y;
            stack.push(std::pair<unsigned int, unsigned int>{x, y});
            do
            {
              // mark as visited by making it transparent, the data is not actually being used for anything else anyway
              *(m_Data + (static_cast<long long>(a) + static_cast<long long>(b) * m_Width) * 4 + 3) = 0;

              a = stack.top().first;
              b = stack.top().second;
              stack.pop();

              minX = a < minX ? a : minX;
              maxX = a > maxX ? a : maxX;
              minY = b < minY ? b : minY;
              maxY = b > maxY ? b : maxY;

              // use 8 way flood fill algorithm to detect the sprite
              // add valid neighbours to the queue
              for (int i = -1; i <= 1; ++i)
                for (int j = -1; j <= 1; ++j)
                  if ((i || j) &&
                    !((a == 0 && i == -1) ||
                      (b == 0 && j == -1) ||
                      (a == static_cast<unsigned int>(m_Width) - 1 && i == 1) ||
                      (b == static_cast<unsigned int>(m_Height) - 1 && j == 1)))
                    if (*(m_Data + (static_cast<long long>(a) + i + (static_cast<long long>(b) + j) * m_Width) * 4 + 3) != 0)
                      stack.push(std::pair<unsigned int, unsigned int>{a + i, b + j});
            }
            while (!stack.empty());

            // add the info
            textureInfo.push_back({
              Vector2(1.0f * minX, 1.0f * minY),
              Vector2(1.0f * maxX - minX, 1.0f * maxY - minY),
              pivot
            });

            if (noOverlap)
              for (a = minX; a <= maxX; ++a)
                for (b = minY; b <= maxY; ++b)
                  *(m_Data + (static_cast<long long>(a) + static_cast<long long>(b) * m_Width) * 4 + 3) = 0;
          }
        }
    }
    DeltaEngine_CORE_INFO("{0} sprites detected in texture \"{1}\"", textureInfo.size(), m_Filepath);

    if (m_Data)
      stbi_image_free(m_Data);

    UpdateMetaFile(m_Filepath + ".info");
  }

  void Texture2D::Slice(TextureInfo info)
  {
    textureInfo.push_back(info);
    UpdateMetaFile(m_Filepath + ".info");
  }

  void Texture2D::SliceAll(unsigned int columns, unsigned int rows, Vector2 pivot)
  {
    textureInfo.clear();
    for (size_t y = 0; y < rows; ++y)
    {
      for (size_t x = 0; x < columns; ++x)
      {
        textureInfo.push_back({
          Vector2(static_cast<float>(m_Width) / columns * x, static_cast<float>(m_Height) / rows * y),
          Vector2(static_cast<float>(m_Width) / columns, static_cast<float>(m_Height) / rows),
          pivot
        });
      }
    }
    UpdateMetaFile(m_Filepath + ".info");

    std::ofstream file{m_Name + ".clip"};
    if (file.is_open())
    {
      for (size_t i = 0; i < textureInfo.size(); ++i)
      {
        file << m_Name << "_i_" << i << std::endl;
        file << "key " << m_Name << std::endl;
        file << "value " << i << std::endl << std::endl;
      }
      file << std::endl << "%" << std::endl;
      file.close();
    }
    else
    {
      DeltaEngine_CORE_ERROR("Failed to create animation clip \"{}\"!", m_Name);
    }
  }

  Vector2 Texture2D::GetOffset(unsigned int index)
  {
    return Vector2(textureInfo[index].offset.x / m_Width, textureInfo[index].offset.y / m_Height);
  }

  Vector2 Texture2D::GetSize(unsigned int index)
  {
    return textureInfo[index].size;
  }

  Vector2 Texture2D::GetPivot(unsigned int index)
  {
    return textureInfo[index].pivot;
  }

  std::string Texture2D::GetName()
  {
    return m_Name;
  }

  std::string Texture2D::GetFilepath()
  {
    return m_Filepath;
  }

  void Texture2D::InitTexture()
  {
    stbi_set_flip_vertically_on_load(0);

    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    unsigned char* m_Data = stbi_load(m_Filepath.c_str(), &m_Width, &m_Height, &m_Channels, 0);

    if (!m_Data)
    {
      DeltaEngine_CORE_ERROR("ERROR: Couldn't create texture {}!", m_Filepath);
      m_Filepath = "";
    }

    LoadMetaFile(m_Filepath + ".info");

    int glWrapMode = GL_REPEAT;
    switch (wrapMode)
    {
    case TextureWrapMode::Repeat:
      glWrapMode = GL_REPEAT;
      break;
    case TextureWrapMode::Mirror:
      glWrapMode = GL_MIRRORED_REPEAT;
      break;
    case TextureWrapMode::Clamp:
      glWrapMode = GL_CLAMP;
      break;
    }

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glWrapMode));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glWrapMode));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Data));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    if (m_Data)
      stbi_image_free(m_Data);

    std::size_t pos;
    pos = m_Name.find(".");
    if (pos != std::string::npos)
      m_Name.erase(pos);

    UpdateMetaFile(m_Filepath + ".info");
  }

  void Texture2D::LoadMetaFile(std::string filepath)
  {
    std::ifstream file;
    DeltaEngine_CORE_TRACE("Loading Texture info file \"{}\"...", filepath.c_str());
    file.open((filepath).c_str());

    std::string str;
    textureInfo.clear();

    if (file.is_open())
    {
      while (file.good())
      {
        file >> str;
        if (!strcmp(str.c_str(), "%"))
          break;
        textureInfo.push_back(TextureInfo());
        file >> str >> textureInfo.back().offset.x >> textureInfo.back().offset.y;
        file >> str >> textureInfo.back().size.x >> textureInfo.back().size.y;
        file >> str >> textureInfo.back().pivot.x >> textureInfo.back().pivot.y;
      }
      int wm = 0;
      file >> str >> wm;
      wrapMode = (TextureWrapMode)wm;
      file.close();
    }
    else
    {
      DeltaEngine_CORE_WARN("Texture info file \"{}\" doesn't exist, creating automatically", filepath.c_str());
      textureInfo.push_back({
        Vector2(0, 0), Vector2(static_cast<float>(m_Width), static_cast<float>(m_Height)), Vector2(0.5f, 0.5f)
      });
      UpdateMetaFile(filepath);
    }
  }

  void Texture2D::UpdateMetaFile(std::string filepath)
  {
    std::ofstream file{filepath.c_str()};
    if (file.is_open())
    {
      for (size_t i = 0; i < textureInfo.size(); ++i)
      {
        file << m_Name << "_i_" << i << std::endl;
        file << "offset " << textureInfo[i].offset.x << " " << textureInfo[i].offset.y << std::endl;
        file << "size " << textureInfo[i].size.x << " " << textureInfo[i].size.y << std::endl;
        file << "pivot " << textureInfo[i].pivot.x << " " << textureInfo[i].pivot.y << std::endl;
        file << std::endl;
      }
      file << "%" << std::endl;
      file << std::endl;
      file << "wrap_mode " << static_cast<int>(wrapMode) << std::endl;
      file.close();
    }
    else
    {
      DeltaEngine_CORE_ERROR("Failed to create info file for texture \"{}\"!", filepath);
    }
  }
}