#include "Mesh.h"
#include <iostream>
#include <algorithm>
#include "ErrorCheck.h"

namespace DeltaEngine
{
  //********************************************************************************
  // VertexBufferLayout 
  //********************************************************************************
#pragma region VertexBufferLayout class
class Mesh::VertexBufferLayout
{
public:
  struct VertexBufferElement
  {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType( unsigned int type )
    {
      switch ( type )
      {
        case GL_FLOAT: return 4;
        case GL_UNSIGNED_INT: return 4;
        case GL_UNSIGNED_BYTE: return 1;
      }
      ASSERT( false );
      return 0;
    }
  };
private:
  std::vector<VertexBufferElement> m_Elements;
  unsigned int m_Stride;
public:
  VertexBufferLayout() : m_Stride { 0 }
  {}

  template<typename T>
  void Push( unsigned int count )
  {
    static_assert( false );
  }

  template<>
  void Push<float>( unsigned int count )
  {
    m_Elements.push_back( { GL_FLOAT, count, false } );
    m_Stride += count * VertexBufferElement::GetSizeOfType( GL_FLOAT );
  }

  template<>
  void Push<unsigned int>( unsigned int count )
  {
    m_Elements.push_back( { GL_UNSIGNED_INT, count, false } );
    m_Stride += count * VertexBufferElement::GetSizeOfType( GL_UNSIGNED_INT );
  }

  template<>
  void Push<unsigned char>( unsigned int count )
  {
    m_Elements.push_back( { GL_UNSIGNED_BYTE, count, true } );
    m_Stride += count * VertexBufferElement::GetSizeOfType( GL_UNSIGNED_BYTE );
  }

  inline const std::vector<VertexBufferElement> GetElements() const
  {
    return m_Elements;
  }
  inline unsigned int GetStride() const
  {
    return m_Stride;
  }
};
#pragma endregion

//********************************************************************************
// VertexBuffer 
//********************************************************************************
#pragma region VertexBuffer class
Mesh::VertexBuffer::VertexBuffer()
{
  GLCall( glGenBuffers( 1, &m_RendererID ) );
}

Mesh::VertexBuffer::~VertexBuffer()
{
  GLCall( glDeleteBuffers( 1, &m_RendererID ) );
}

void Mesh::VertexBuffer::InitData( const float *data, unsigned int size, bool dynamic )
{
  GLCall( glBindBuffer( GL_ARRAY_BUFFER, m_RendererID ) );
  GLCall( glBufferData( GL_ARRAY_BUFFER, size, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW ) );
}

void Mesh::VertexBuffer::InitSubData( const float *data, unsigned int offset, unsigned int size )
{
  GLCall( glBufferSubData( GL_ARRAY_BUFFER, offset, size, data ) );
}

void Mesh::VertexBuffer::Bind() const
{
  GLCall( glBindBuffer( GL_ARRAY_BUFFER, m_RendererID ) );
}

void Mesh::VertexBuffer::Unbind() const
{
  GLCall( glBindBuffer( GL_ARRAY_BUFFER, 0 ) );
}
#pragma endregion

//********************************************************************************
// VertexArray 
//********************************************************************************
#pragma region VertexArray class
Mesh::VertexArray::VertexArray()
{
  GLCall( glGenVertexArrays( 1, &m_RendererID ) );
}

Mesh::VertexArray::~VertexArray()
{
  GLCall( glDeleteVertexArrays( 1, &m_RendererID ) );
}

void Mesh::VertexArray::AddBuffer( const VertexBuffer &vb, const VertexBufferLayout &layout )
{
  Bind();
  vb.Bind();
  const auto &elements = layout.GetElements();
  unsigned int offset = 0;
  for ( unsigned int i = 0; i < elements.size(); ++i )
  {
    const auto &element = elements[i];
    GLCall( glEnableVertexAttribArray( i ) );
    //this line below is to avoid a warning related to OpenGL API design flaw for this particular function "glVertexAttribPointer"
    void const *pointer = static_cast<char const *>( 0 ) + offset;
    GLCall( glVertexAttribPointer( i, element.count, element.type, element.normalized, layout.GetStride(), (const void *) pointer ) );
    offset += element.count * VertexBufferLayout::VertexBufferElement::GetSizeOfType( element.type );
  }
}

void Mesh::VertexArray::Bind() const
{
  GLCall( glBindVertexArray( m_RendererID ) );
}

void Mesh::VertexArray::Unbind() const
{
  GLCall( glBindVertexArray( 0 ) );
}
#pragma endregion

//********************************************************************************
// IndexBuffer 
//********************************************************************************
#pragma region IndexBuffer class
Mesh::IndexBuffer::IndexBuffer()
  : m_Count
{
  0
}
{
  GLCall( glGenBuffers( 1, &m_RendererID ) );
}

Mesh::IndexBuffer::~IndexBuffer()
{
  GLCall( glDeleteBuffers( 1, &m_RendererID ) );
}

void Mesh::IndexBuffer::InitData( const unsigned int *data, unsigned int count, bool dynamic )
{
  m_Count = count;

  GLCall( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_RendererID ) );
  GLCall( glBufferData( GL_ELEMENT_ARRAY_BUFFER, count * sizeof( unsigned int ), data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW ) );
}

void Mesh::IndexBuffer::Bind() const
{
  GLCall( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_RendererID ) );
}

void Mesh::IndexBuffer::Unbind() const
{
  GLCall( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ) );
}
#pragma endregion

//********************************************************************************
// Mesh 
//********************************************************************************
#pragma region Mesh class
std::vector<float> Mesh::VerticesDataFormat()
{
  AssertProperties();
  std::vector<float> coords;
  for ( unsigned int i = 0; i < verticesCount; ++i )
  {
    //vertex position
    coords.push_back( vertices[i].x );
    coords.push_back( vertices[i].y );
    coords.push_back( vertices[i].z );

    //color
    coords.push_back( colors[i].r );
    coords.push_back( colors[i].g );
    coords.push_back( colors[i].b );
    coords.push_back( colors[i].a );

    //texture coordinates
    coords.push_back( texCoords[i].x );
    coords.push_back( texCoords[i].y );
  };
  return coords;
}
void Mesh::AssertProperties()
{
  verticesCount = static_cast<unsigned int>( vertices.size() );
  //ensure there are enough colors for each vertex
  if ( colors.size() < verticesCount )
  {
    for ( size_t i = colors.size(); i < verticesCount; i++ )
    {
      colors.push_back( Color() );
    }
  }
  //ensure there are enough texcoords for each vertex
  if ( texCoords.size() < verticesCount )
  {
    for ( size_t i = texCoords.size(); i < verticesCount; i++ )
    {
      texCoords.push_back( Vector2() );
    }
  }
}

// default mesh is a unit square (quad)
Mesh::Mesh(bool dynamic)
{
  AssertProperties();

  isDynamic = dynamic;

  vao.Bind();
  vbo.InitData( VerticesDataFormat().data(), static_cast<unsigned int>( vertices.size() * 9 * sizeof( float ) ), isDynamic );
  if ( indices.size() )
    ibo.InitData( indices.data(), static_cast<unsigned int>( indices.size() ), isDynamic );

  VertexBufferLayout layout;
  layout.Push<float>( 3 );
  layout.Push<float>( 4 );
  layout.Push<float>( 2 );
  vao.AddBuffer( vbo, layout );

  if ( indices.size() )
    ibo.Bind();

  vao.Unbind();
  vbo.Unbind();
  if ( indices.size() )
    ibo.Unbind();
}
void Mesh::SetVertices( std::vector<Vector3> v )
{
  vertices = v;
  AssertProperties();
  vbo.InitData( VerticesDataFormat().data(), static_cast<unsigned int>( vertices.size() * 9 * sizeof( float ) ) );
}
void Mesh::SetColors( std::vector<Color> c )
{
  colors = c;
  AssertProperties();
  vbo.InitData( VerticesDataFormat().data(), static_cast<unsigned int>( vertices.size() * 9 * sizeof( float ) ) );
}
void Mesh::SetUVs( std::vector<Vector2> v )
{
  texCoords = v;
  AssertProperties();
  vbo.InitData( VerticesDataFormat().data(), static_cast<unsigned int>( vertices.size() * 9 * sizeof( float ) ) );
}
void Mesh::SetIndices( std::vector<unsigned int> i )
{
  indices = i;

  if ( indices.size() )
    ibo.InitData( indices.data(), static_cast<unsigned int>( indices.size() ) );

  VertexBufferLayout layout;
  layout.Push<float>( 3 );
  layout.Push<float>( 4 );
  layout.Push<float>( 2 );
  vao.AddBuffer( vbo, layout );

  if ( indices.size() )
    ibo.Bind();
}

void Mesh::Draw()
{
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

  if ( indices.size() )
  {
    ibo.InitData( indices.data(), static_cast<unsigned int>( indices.size() ), isDynamic );
    ibo.Bind();
  }

  vbo.InitData( VerticesDataFormat().data(), static_cast<unsigned int>( vertices.size() * 9 * sizeof( float ) ), isDynamic );
  vbo.Bind();

  vbo.Unbind();

  vao.Bind();

  if ( indices.size() )
  {
    GLCall( glDrawElements( GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr ) );
  }
  else
  {
    GLCall( glDrawArrays( GL_TRIANGLES, 0, static_cast<unsigned int>( vertices.size() ) ) );
  }

  vao.Unbind();
  if ( indices.size() )
    ibo.Unbind();
}
void Mesh::DrawWireframe()
{
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

  if ( indices.size() )
  {
    ibo.InitData( indices.data(), static_cast<unsigned int>( indices.size() ) );
    ibo.Bind();
  }

  vbo.InitData( VerticesDataFormat().data(), static_cast<unsigned int>( vertices.size() * 9 * sizeof( float ) ), isDynamic );
  vbo.Bind();

  vbo.Unbind();

  vao.Bind();

  if ( indices.size() )
  {
    GLCall( glDrawElements( GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr ) );
  }
  else
  {
    GLCall( glDrawArrays( GL_TRIANGLES, 0, static_cast<unsigned int>( vertices.size() ) ) );
  }

  vao.Unbind();
  if ( indices.size() )
    ibo.Unbind();
}

Mesh *quad;
Mesh *line;
Mesh *txtm;

void Mesh::Init()
{
  // temp vectors
  std::vector<Vector3> verts;
  std::vector<Vector2> uvs;
  std::vector<unsigned int> inds;

  // init quad
  quad = new Mesh();

  // init line with empty static mesh
  line = new Mesh();

  // init text with empty dynamic mesh
  txtm = new Mesh(true);

  verts.push_back( Vector3( -0.5f, 0.5f, 0.0f ) );
  verts.push_back( Vector3( 0.5f, 0.5f, 0.0f ) );
  verts.push_back( Vector3( 0.5f, -0.5f, 0.0f ) );
  verts.push_back( Vector3( -0.5f, -0.5f, 0.0f ) );
  quad->SetVertices( verts );
  txtm->SetVertices( verts );

  uvs.push_back( Vector2( 0.0f, 0.0f ) );
  uvs.push_back( Vector2( 1.0f, 0.0f ) );
  uvs.push_back( Vector2( 1.0f, 1.0f ) );
  uvs.push_back( Vector2( 0.0f, 1.0f ) );
  quad->SetUVs( uvs );
  txtm->SetUVs( uvs );

  inds.push_back( 0 );
  inds.push_back( 1 );
  inds.push_back( 2 );
  inds.push_back( 2 );
  inds.push_back( 3 );
  inds.push_back( 0 );
  quad->SetIndices( inds );
  txtm->SetIndices( inds );
}

void Mesh::Exit()
{
  delete quad;
  delete line;
  delete txtm;
}

void Mesh::DrawQuad( bool wireframe )
{
  quad->vertices[0] = Vector3( -0.5f, 0.5f, 0.0f );
  quad->vertices[1] = Vector3( 0.5f, 0.5f, 0.0f );
  quad->vertices[2] = Vector3( 0.5f, -0.5f, 0.0f );
  quad->vertices[3] = Vector3( -0.5f, -0.5f, 0.0f );
  quad->texCoords[0] = Vector2( 0, 0 );
  quad->texCoords[1] = Vector2( 1, 0 );
  quad->texCoords[2] = Vector2( 1, 1 );
  quad->texCoords[3] = Vector2( 0, 1 );
  if ( wireframe )
    quad->DrawWireframe();
  else
    quad->Draw();
}

void Mesh::DrawQuad( Vector2 offset, Vector2 tiling, Vector2 pivot )
{
  quad->vertices[0] = Vector3( 0.0f, 1.0f, 0.0f ) - pivot;
  quad->vertices[1] = Vector3( 1.0f, 1.0f, 0.0f ) - pivot;
  quad->vertices[2] = Vector3( 1.0f, 0.0f, 0.0f ) - pivot;
  quad->vertices[3] = Vector3( 0.0f, 0.0f, 0.0f ) - pivot;
  quad->texCoords[0] = Vector2( offset.x, offset.y );
  quad->texCoords[1] = Vector2( offset.x + tiling.x, offset.y );
  quad->texCoords[2] = Vector2( offset.x + tiling.x, offset.y + tiling.y );
  quad->texCoords[3] = Vector2( offset.x, offset.y + tiling.y );
  quad->Draw();
}

void Mesh::DrawLine( Vector3 start, Vector3 end )
{
  std::vector<Vector3> verts;
  verts.push_back( start );
  verts.push_back( end );

  line->SetVertices( verts );
  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

  line->vbo.InitData( line->VerticesDataFormat().data(), static_cast<unsigned int>( line->vertices.size() * 9 * sizeof( float ) ) );
  line->vbo.Bind();

  line->vbo.Unbind();

  line->vao.Bind();

  GLCall( glDrawArrays( GL_LINE_STRIP, 0, static_cast<unsigned int>( line->vertices.size() ) ) );

  line->vao.Unbind();
}

void Mesh::DrawLines( std::vector<std::pair<Vector3, Vector3>> startEndPair )
{
  std::vector<Vector3> verts;
  for ( const auto& a : startEndPair )
  {
    verts.push_back( a.first );
    verts.push_back( a.second );
  }

  line->SetVertices( verts );
  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

  line->vbo.InitData( line->VerticesDataFormat().data(), static_cast<unsigned int>( line->vertices.size() * 9 * sizeof( float ) ) );
  line->vbo.Bind();

  line->vbo.Unbind();

  line->vao.Bind();

  GLCall( glDrawArrays( GL_LINES, 0, static_cast<unsigned int>( line->vertices.size() ) ) );

  line->vao.Unbind();
}
void Mesh::DrawTextMesh( Font* font, std::string text, float size, bool wireframe)
{
  float x = 0;
  float y = 0;

  float scale = 0.01f * size;

  glActiveTexture(GL_TEXTURE0);
  txtm->vao.Bind();

  wireframe ? glDisable(GL_BLEND) : glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
  // iterate through all characters
  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++)
  {
    CharacterInfo ch = font->characterInfo()[*c];

    float xpos = x + ch.bearing.x * scale;
    float ypos = y - (ch.size.y - ch.bearing.y) * scale;

    float w = ch.size.x * scale;
    float h = ch.size.y * scale;
    // update VBO for each character
    txtm->vertices[0] = Vector3(xpos, ypos + h, 0.0f);
    txtm->vertices[1] = Vector3(xpos + w, ypos + h, 0.0f);
    txtm->vertices[2] = Vector3(xpos + w, ypos, 0.0f);
    txtm->vertices[3] = Vector3(xpos, ypos, 0.0f);

    std::vector<float> v = txtm->VerticesDataFormat();

    // render glyph texture over quad
    if (!wireframe)
      glBindTexture(GL_TEXTURE_2D, ch.textureID);
    // update content of vbo memory
    txtm->vbo.Bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, v.size() * sizeof(float), v.data());

    txtm->vbo.Unbind();
    // render quad
    GLCall( glDrawElements( GL_TRIANGLES, txtm->ibo.GetCount(), GL_UNSIGNED_INT, nullptr ) );
    // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
    x += (ch.advance >> 6)* scale; // bitshift by 6 to get value in pixels (2^6 = 64)
  }
  txtm->vao.Unbind();
  if (!wireframe)
    glBindTexture(GL_TEXTURE_2D, 0);
}

#pragma endregion
}
