#include "Texture.h"
#include "ErrorCheck.h"
#include "Core/Debugging/Logger/Log.h"
#include <stb_image.h>
#include "DEpch.h"
#include <stack>

namespace DeltaEngine
{
Texture2D::Texture2D( std::string filepath )
  : m_RendererID{ 0 }, m_Channels{ 0 }, m_Width{ 0 }, m_Height{ 0 }, m_Filepath{ filepath }, m_Name{ filepath }
{
  InitTexture(filepath);
}

Texture2D::~Texture2D()
{
  DeltaEngine_CORE_INFO( "Deleting Texture" );
  GLCall( glDeleteTextures( 1, &m_RendererID ) );
  DeltaEngine_CORE_INFO( "Texture deleted" );
}

void Texture2D::Bind( unsigned int slot ) const
{
  glActiveTexture( GL_TEXTURE0 + slot );
  GLCall( glBindTexture( GL_TEXTURE_2D, m_RendererID ) );
}

void Texture2D::Unbind() const
{
  GLCall( glBindTexture( GL_TEXTURE_2D, 0 ) );
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

void Texture2D::AutoSlice()
{
  textureInfo.clear();
  unsigned char* m_Data = stbi_load(m_Filepath.c_str(), &m_Width, &m_Height, &m_Channels, 0);

  std::cerr << "auto slicing texture: " << m_Filepath <<std::endl;

  // only auto slice if there is an alpha channel
  if (m_Channels != 4)
  {
    textureInfo.push_back({
      Vector2(0, 0), Vector2(static_cast<float>(m_Width), static_cast<float>(m_Height)), Vector2(0.5f, 0.5f) });
  }
  else
  {
    std::vector<std::pair<unsigned int, unsigned int>> visited;
    std::vector<std::pair<unsigned int, unsigned int>> sprite;
    std::stack<std::pair<unsigned int, unsigned int>> stack;
    int c = 0;
    for (unsigned int y = 0; y < (unsigned int)m_Height; ++y)
    {
      for (unsigned int x = 0; x < (unsigned int)m_Width; ++x)
      {
        // check for a non-transparent pixel
        if (*(m_Data + ((long long)x + (long long)y * m_Width) * 4 + 3) != 0)
        {
          if (std::find(visited.begin(), visited.end(), std::pair<unsigned int, unsigned int>{x, y}) != visited.end())
            continue;
          unsigned int a = x, b = y;
          stack.push(std::pair<unsigned int, unsigned int>{x, y});
          do
          {
            a = stack.top().first;
            b = stack.top().second;
            stack.pop();

            // check if the pixel has been visited
            if (std::find(visited.begin(), visited.end(), std::pair<unsigned int, unsigned int>{a, b}) == visited.end())
            {
              sprite.push_back(std::pair<unsigned int, unsigned int>{a, b});
              visited.push_back(std::pair<unsigned int, unsigned int>{a, b});
              // use 8 way flood fill algorithm to detect the sprite
              // add valid neighbours to the queue
              for (int j = -1; j <= 1; ++j)
                for (int i = -1; i <= 1; ++i)
                  if ((i || j) &&
                    !((a == 0 && i == -1) ||
                      (b == 0 && j == -1) ||
                      (a == (unsigned int)m_Width - 1 && i == 1) ||
                      (b == (unsigned int)m_Height - 1 && j == 1)))
                  {
                    if (*(m_Data + ((long long)a + i + ((long long)b + j) * m_Width) * 4 + 3) != 0)
                    {
                      stack.push(std::pair<unsigned int, unsigned int>{a + i, b + j});
                    }
                  }
              std::cerr << sprite.size() << std::endl;
            }
          } while (!stack.empty());

          // get the bounding box from the sprite vector
          unsigned int minX = m_Width, maxX = 0, minY = m_Height, maxY = 0;
          for (auto& spriteCoords : sprite)
          {
            minX = spriteCoords.first < minX ? spriteCoords.first : minX;
            maxX = spriteCoords.first > maxX ? spriteCoords.first : maxX;
            minY = spriteCoords.second < minY ? spriteCoords.second : minY;
            maxY = spriteCoords.second > maxY ? spriteCoords.second : maxY;
          }
          // add the info
          textureInfo.push_back({
            Vector2(1.0f * minX, 1.0f * minY),
            Vector2(1.0f * maxX - minX, 1.0f * maxY - minY),
            Vector2(0.5f, 0.5f) });
          std::cerr << c++ << ": " << minX << ", " << minY << ", " << maxX << ", " << maxY << std::endl;
          // clear the sprite vector, leave visited vector untouched
          sprite.clear();
        }
      }
    }
  }
  std::cerr << textureInfo.size() << " Sprites Detected" << std::endl;

  UpdateMetaFile( m_Filepath + ".info" );
}

void Texture2D::Slice( TextureInfo info )
{
  textureInfo.push_back( info );
  UpdateMetaFile( m_Filepath + ".info" );
}

void Texture2D::SliceAll( unsigned int columns, unsigned int rows )
{
  textureInfo.clear();
  for ( size_t y = 0; y < rows; ++y )
  {
    for ( size_t x = 0; x < columns; ++x )
    {
      textureInfo.push_back( {
        Vector2( static_cast<float>( m_Width ) / columns * x, static_cast<float>( m_Height ) / rows * y ),
        Vector2( static_cast<float>( m_Width ) / columns, static_cast<float>( m_Height ) / rows ),
        Vector2( 0.5f, 0.5f ) } );
    }
  }
  UpdateMetaFile( m_Filepath + ".info" );

  std::ofstream file{ m_Name + ".clip" };
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

Vector2 Texture2D::GetOffset( unsigned int index )
{
  return Vector2( textureInfo[index].offset.x / m_Width, textureInfo[index].offset.y / m_Height );
}
Vector2 Texture2D::GetSize( unsigned int index )
{
  return textureInfo[index].size;
}
Vector2 Texture2D::GetPivot( unsigned int index )
{
  return textureInfo[index].pivot;
}
std::string Texture2D::GetName()
{
  return m_Name;
}

void Texture2D::InitTexture(std::string filepath)
{
  stbi_set_flip_vertically_on_load(0);

  GLCall(glGenTextures(1, &m_RendererID));
  GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

  unsigned char* m_Data = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_Channels, 0);

  if (!m_Data)
  {
    DeltaEngine_CORE_ERROR("ERROR: Couldn't create texture {}!", filepath);
    m_Filepath = "";
  }

  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

  GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Data));
  GLCall(glGenerateMipmap(GL_TEXTURE_2D));
  GLCall(glBindTexture(GL_TEXTURE_2D, 0));

  if (m_Data)
    stbi_image_free(m_Data);

  std::size_t pos;
  pos = m_Name.find(".");
  if (pos != std::string::npos)
    m_Name.erase(pos);

  LoadMetaFile(filepath + ".info");
}

void Texture2D::LoadMetaFile( std::string filepath )
{
  std::ifstream file;
  DeltaEngine_CORE_TRACE( "Loading Texture info file \"{}\"...", filepath.c_str() );
  file.open( ( filepath ).c_str() );

  std::string str;
  textureInfo.clear();

  if ( file.is_open() )
  {
    while ( file.good() )
    {
      textureInfo.push_back( TextureInfo() );
      file >> str;
      file >> str >> textureInfo.back().offset.x >> textureInfo.back().offset.y;
      file >> str >> textureInfo.back().size.x >> textureInfo.back().size.y;
      file >> str >> textureInfo.back().pivot.x >> textureInfo.back().pivot.y;
    }
    file.close();
  }
  else
  {
    DeltaEngine_CORE_WARN( "Texture info file \"{}\" doesn't exist, creating automatically", filepath.c_str() );
    textureInfo.push_back( {
      Vector2( 0, 0 ), Vector2( static_cast<float>( m_Width ), static_cast<float>( m_Height ) ), Vector2( 0.5f, 0.5f ) } );
    UpdateMetaFile( filepath );
  }
}
void Texture2D::UpdateMetaFile( std::string filepath )
{
  std::ofstream file { filepath.c_str() };
  if ( file.is_open() )
  {
    for ( size_t i = 0; i < textureInfo.size(); ++i )
    {
      file << m_Name << "_i_" << i << std::endl;
      file << "offset " << textureInfo[i].offset.x << " " << textureInfo[i].offset.y << std::endl;
      file << "size " << textureInfo[i].size.x << " " << textureInfo[i].size.y << std::endl;
      file << "pivot " << textureInfo[i].pivot.x << " " << textureInfo[i].pivot.y << std::endl;
      file << std::endl;
    }
    file << std::endl << "%" << std::endl;
    file.close();
  }
  else
  {
    DeltaEngine_CORE_ERROR( "Failed to create info file for texture \"{}\"!", filepath );
  }
}
}
