#pragma once

#include <GL/glew.h>
#include "Math/Color.h"
#include <vector>

namespace DeltaEngine
{
	class Mesh
	{
		class VertexBufferLayout;

		class VertexBuffer
		{
			unsigned int m_RendererID;
		public:
			VertexBuffer();
			~VertexBuffer();

			void InitData(const void* data, unsigned int size);

			void Bind() const;
			void Unbind() const;
		};

		class VertexArray
		{
			unsigned int m_RendererID;
		public:
			VertexArray();
			~VertexArray();

			void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

			void Bind() const;
			void Unbind() const;
		};

		class IndexBuffer
		{
			unsigned int m_RendererID;
			unsigned int m_Count;
		public:
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

		float* VerticesDataFormat();
		void AssertProperties();
	public:
		Mesh();
		void Draw();

		void SetVertices();
	};
}
