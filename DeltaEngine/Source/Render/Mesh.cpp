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

			static unsigned int GetSizeOfType(unsigned int type)
			{
				switch (type)
				{
				case GL_FLOAT: return 4;
				case GL_UNSIGNED_INT: return 4;
				case GL_UNSIGNED_BYTE: return 1;
				}
				ASSERT(false);
				return 0;
			}
		};
	private:
		std::vector<VertexBufferElement> m_Elements;
		unsigned int m_Stride;
	public:
		VertexBufferLayout() : m_Stride{ 0 } {}

		template<typename T>
		void Push(unsigned int count)
		{
			static_assert(false);
		}

		template<>
		void Push<float>(unsigned int count)
		{
			m_Elements.push_back({ GL_FLOAT, count, false });
			m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
		}

		template<>
		void Push<unsigned int>(unsigned int count)
		{
			m_Elements.push_back({ GL_UNSIGNED_INT, count, false });
			m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
		}

		template<>
		void Push<unsigned char>(unsigned int count)
		{
			m_Elements.push_back({ GL_UNSIGNED_BYTE, count, true });
			m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
		}

		inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
		inline unsigned int GetStride() const { return m_Stride; }
	};
	#pragma endregion

	//********************************************************************************
	// VertexBuffer 
	//********************************************************************************
	#pragma region VertexBuffer class
	Mesh::VertexBuffer::VertexBuffer()
	{
		GLCall(glGenBuffers(1, &m_RendererID));
	}

	Mesh::VertexBuffer::~VertexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}

	void Mesh::VertexBuffer::InitData(const void* data, unsigned int size)
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}

	void Mesh::VertexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	}

	void Mesh::VertexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
	#pragma endregion

	//********************************************************************************
	// VertexArray 
	//********************************************************************************
	#pragma region VertexArray class
	Mesh::VertexArray::VertexArray()
	{
		GLCall(glGenVertexArrays(1, &m_RendererID));
	}

	Mesh::VertexArray::~VertexArray()
	{
		GLCall(glDeleteVertexArrays(1, &m_RendererID));
	}

	void Mesh::VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
	{
		Bind();
		vb.Bind();
		const auto& elements = layout.GetElements();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++)
		{
			const auto& element = elements[i];
			GLCall(glEnableVertexAttribArray(i));
			//this line below is to avoid a warning related to OpenGL API design flaw for this particular function "glVertexAttribPointer"
			void const* pointer = static_cast<char const*>(0) + offset;
			GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)pointer));
			offset += element.count * VertexBufferLayout::VertexBufferElement::GetSizeOfType(element.type);
		}
	}

	void Mesh::VertexArray::Bind() const
	{
		GLCall(glBindVertexArray(m_RendererID));
	}

	void Mesh::VertexArray::Unbind() const
	{
		GLCall(glBindVertexArray(0));
	}
	#pragma endregion

	//********************************************************************************
	// IndexBuffer 
	//********************************************************************************
	#pragma region IndexBuffer class
	Mesh::IndexBuffer::IndexBuffer()
		: m_Count { 0 }
	{
		//ASSERT(sizeof(unsigned int) == sizeof(GLuint));

		GLCall(glGenBuffers(1, &m_RendererID));
	}

	Mesh::IndexBuffer::~IndexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}

	void Mesh::IndexBuffer::InitData(const unsigned int* data, unsigned int count)
	{
		m_Count = count;

		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
	}

	void Mesh::IndexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	}

	void Mesh::IndexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
	#pragma endregion

	//********************************************************************************
	// Mesh 
	//********************************************************************************
	#pragma region Mesh class
	float* Mesh::VerticesDataFormat()
	{
		std::vector<float> coords;
		for (unsigned int i = 0; i < verticesCount; ++i)
		{
			//position
			coords.push_back(vertices[i].x);
			coords.push_back(vertices[i].y);
			coords.push_back(vertices[i].x);

			//color
			coords.push_back(colors[i].r);
			coords.push_back(colors[i].g);
			coords.push_back(colors[i].b);
			coords.push_back(colors[i].a);

			//texture
			coords.push_back(texCoords[i].x);
			coords.push_back(texCoords[i].y);
		};
		return coords.data();
	}
	void Mesh::AssertProperties()
	{
		verticesCount = static_cast<unsigned int>(vertices.size());
		//ensure there are enough colors for each vertex
		if (colors.size() < verticesCount)
		{
			for (size_t i = colors.size(); i < verticesCount; i++)
			{
				colors.push_back(Color());
			}
		}
		//ensure there are enough texcoords for each vertex
		if (texCoords.size() < verticesCount)
		{
			for (size_t i = texCoords.size(); i < verticesCount; i++)
			{
				texCoords.push_back(Vector2());
			}
		}
	}

	// default mesh is a unit square
	Mesh::Mesh()
	{
		vertices.push_back(Vector3(-0.5f,  0.5f, 0.0f));
		vertices.push_back(Vector3( 0.5f,  0.5f, 0.0f));
		vertices.push_back(Vector3( 0.5f, -0.5f, 0.0f));
		vertices.push_back(Vector3(-0.5f, -0.5f, 0.0f));

		colors.push_back(Color::white());
		colors.push_back(Color::white());
		colors.push_back(Color::white());
		colors.push_back(Color::white());

		texCoords.push_back(Vector2(0.0f, 0.0f));
		texCoords.push_back(Vector2(1.0f, 0.0f));
		texCoords.push_back(Vector2(1.0f, 1.0f));
		texCoords.push_back(Vector2(0.0f, 1.0f));

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(2);
		indices.push_back(3);
		indices.push_back(0);

		AssertProperties();

		vbo.InitData(VerticesDataFormat(), static_cast<unsigned int>(vertices.size() * 9 * sizeof(float)));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(4);
		layout.Push<float>(2);
		vao.AddBuffer(vbo, layout);

		ibo.InitData(indices.data(), 6);
		ibo.Bind();

		vao.Unbind();
		vbo.Unbind();
		ibo.Unbind();
	}

	void Mesh::Draw()
	{
		ibo.InitData(indices.data(), static_cast<unsigned int>(indices.size()));
		ibo.Bind();

		vbo.InitData(VerticesDataFormat(), static_cast<unsigned int>(vertices.size() * 9 * sizeof(float)));
		vbo.Bind();

		vbo.Unbind();

		vao.Bind();

		GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr));

		vao.Unbind();
	}

	void Mesh::SetVertices()
	{
		vertices.clear();
		vertices.push_back(Vector3{ -1.5f, -1.5f, 0.0f });
		vertices.push_back(Vector3{ -0.5f, -1.5f, 0.0f });
		vertices.push_back(Vector3{ -0.5f,  -0.5f, 0.0f });
		vertices.push_back(Vector3{ -1.5f,  -0.5f, 0.0f });
	}
	#pragma endregion
}
