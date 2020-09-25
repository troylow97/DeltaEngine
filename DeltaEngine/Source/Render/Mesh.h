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

			void InitData(const float* data, unsigned int size);

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

		std::vector<float> VerticesDataFormat();
		void AssertProperties();
	public:
		Mesh();
		void Draw();

		// commonly used mesh
		static Mesh* quad;

		static void InitMesh();
		static void Exit();
		static void DrawQuad();
		static void DrawQuad(float offsetX, float offsetY, float tileX, float tileY);
	};
}
