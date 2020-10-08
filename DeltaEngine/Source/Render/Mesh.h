#pragma once

#include <GL/glew.h>
#include "Core/Math/Color.h"
#include <vector>

namespace DeltaEngine
{
	class Mesh
	{
		class VertexBufferLayout;

		class VertexBuffer
		{
		public:
			unsigned int m_RendererID;
			VertexBuffer();
			~VertexBuffer();

			void InitData(const float* data, unsigned int size, bool dynamic = false);

			void Bind() const;
			void Unbind() const;
		};

		class VertexArray
		{
		public:
			unsigned int m_RendererID;
			VertexArray();
			~VertexArray();

			void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

			void Bind() const;
			void Unbind() const;
		};

		class IndexBuffer
		{
		public:
			unsigned int m_RendererID;
			unsigned int m_Count;
			IndexBuffer();
			~IndexBuffer();

			void InitData(const unsigned int* data, unsigned int count);

			void Bind() const;
			void Unbind() const;

			inline unsigned int GetCount() const { return m_Count; }
		};

		VertexArray vao;
		VertexBuffer vbo;
		IndexBuffer ibo;

		const unsigned int MAX_VERTICES = 65534;

		unsigned int verticesCount;

		std::vector<Vector3> vertices;
		std::vector<Color> colors;
		std::vector<Vector2> texCoords;
		//std::vector<Vector3> normals;
		std::vector<unsigned int> indices;

		bool dynamic = false;
		bool useSubdata = false;

		std::vector<float> VerticesDataFormat();
		void AssertProperties();
	public:
		Mesh();

		void SetVertices(std::vector<Vector3> vertices);
		void SetColors(std::vector<Color> colors);
		void SetUVs(std::vector<Vector2> uvs);
		void SetIndices(std::vector<unsigned int> indices);
		void MarkDynamic();

		void Draw();
		void DrawWireframe();

		static void Init();
		static void Exit();
		static void DrawQuad(bool wireframe = false);
		static void DrawQuad(Vector2 offset, Vector2 tiling, Vector2 pivot);
		static void DrawLine(Vector3 start, Vector3 end);
		static void DrawLines(std::vector<std::pair<Vector3, Vector3>> startEndPair);
	};
}
