#include "Texture.h"
#include "ErrorCheck.h"
#include "Core/Debugging/Logger/Log.h"
#include <stb_image.h>
#include "DEpch.h"

namespace DeltaEngine
{
Texture2D::Texture2D( std::string filepath )
  : m_RendererID { 0 }, m_Data { nullptr }, m_Width { 0 }, m_Height { 0 }, m_Filepath { filepath }, m_Name { filepath }
{
  stbi_set_flip_vertically_on_load( 0 );

  GLCall( glGenTextures( 1, &m_RendererID ) );
  GLCall( glBindTexture( GL_TEXTURE_2D, m_RendererID ) );

  int channels;

  m_Data = stbi_load( filepath.c_str(), &m_Width, &m_Height, &channels, 0 );

  if ( !m_Data )
  {
    DeltaEngine_CORE_ERROR( "ERROR: Couldn't create texture {}!", filepath );
    m_Filepath = "";
  }

  GLCall( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ) );
  GLCall( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) );
  GLCall( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT ) );
  GLCall( glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT ) );

  GLCall( glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Data ) );
  GLCall( glGenerateMipmap( GL_TEXTURE_2D ) );
  GLCall( glBindTexture( GL_TEXTURE_2D, 0 ) );

  if ( m_Data )
    stbi_image_free( m_Data );

  std::size_t pos;
  pos = m_Name.find( "." );
  if ( pos != std::string::npos )
    m_Name.erase( pos );

  LoadMetaFile( filepath + ".info" );
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
    file.close();
  }
  else
  {
    DeltaEngine_CORE_ERROR( "Failed to create info file for texture \"{}\"!", filepath );
  }
}
}
