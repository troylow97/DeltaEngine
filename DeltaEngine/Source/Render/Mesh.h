/**********************************************************************************
* \file   Mesh.h
* \brief  The file contains interface of Mesh class.
* \author Ong, Graeme,   100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include <GL/glew.h>
#include "Core/Math/DE_Math.h"
#include "DEpch.h"
#include "Font.h"

namespace DeltaEngine
{
  class Mesh
  {
    class VertexBufferLayout;

    class VertexBuffer
    {
    public:
      unsigned m_RendererID;
      VertexBuffer();
      ~VertexBuffer();

      void InitData(const float* data, unsigned size, bool dynamic = false);
      void InitSubData(const float* data, unsigned offset, unsigned size);

      void Bind() const;
      void Unbind() const;
    };

    class VertexArray
    {
      unsigned vertexAttribArrayCount = 0;
    public:
      unsigned m_RendererID;
      VertexArray();
      ~VertexArray();

      void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, bool instanced = false, int startCount = -1);

      void Bind() const;
      void Unbind() const;
    };

    class IndexBuffer
    {
    public:
      unsigned m_RendererID;
      unsigned m_Count;
      IndexBuffer();
      ~IndexBuffer();

      void InitData(const unsigned* data, unsigned count, bool dynamic = false);

      void Bind() const;
      void Unbind() const;

      unsigned GetCount() const
      {
        return m_Count;
      }
    };

    VertexArray vao;
    VertexBuffer vbo, ivbo;
    IndexBuffer ibo;

    const unsigned MAX_VERTICES = 65534;

    unsigned verticesCount;

    unsigned instances;
    std::vector<float> instanceData;

    std::vector<Vector3> vertices;
    std::vector<Color> colors;
    std::vector<Vector2> texCoords;
    //std::vector<Vector3> normals;
    std::vector<unsigned> indices;

    bool isDynamic = false;
    bool useInstancing = false;
    bool useSubdata = false;

    std::vector<float> VerticesDataFormat();
    void AssertProperties();
  public:
    Mesh(bool dynamic = false);

    void SetVertices(std::vector<Vector3> vertices);
    void SetColors(std::vector<Color> colors);
    void SetUVs(std::vector<Vector2> uvs);
    void SetIndices(std::vector<unsigned> indices);

    void Draw();

    static void Init();
    static void Exit();


    static void DrawQuadInst(unsigned count, std::vector<float> instData, std::vector<float> locations,
      Vector2 offset = Vector2(0,0), Vector2 tiling = Vector2(1,1), Vector2 pivot = Vector2(.5f, .5f));

    static void DrawQuad(
      Vector2 offset = Vector2(0, 0),
      Vector2 tiling = Vector2(1, 1),
      Vector2 pivot = Vector2(.5f, .5f),
      bool wireframe = false);
    static void DrawLine(Vector3 start, Vector3 end);
    static void DrawLines(std::vector<std::pair<Vector3, Vector3>> startEndPair);
    static void DrawTextMesh(Font* font, std::string text, float size, bool wireframe = false);
  };
}
