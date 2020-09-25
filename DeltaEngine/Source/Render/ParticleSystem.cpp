#include "ParticleSystem.h"
#include "Core/Math/Math.h"
#include "Core/Math/Random.h"
#include "ErrorCheck.h"

namespace DeltaEngine
{
	//********************************************************************************
	// VertexBufferLayout 
	//********************************************************************************
	#pragma region VertexBufferLayout class
	class ParticleSystem::VertexBufferLayout
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
			m_Elements[m_Elements.size() - 1].normalized = true;
		}

		inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
		inline unsigned int GetStride() const { return m_Stride; }
		inline void Clear() {
			m_Elements.clear();
			m_Stride = 0;
		}
	};
	#pragma endregion

	//********************************************************************************
	// VertexBuffer 
	//********************************************************************************
	#pragma region VertexBuffer class
	ParticleSystem::VertexBuffer::VertexBuffer()
	{
		GLCall(glGenBuffers(1, &m_RendererID));
	}

	ParticleSystem::VertexBuffer::~VertexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}

	void ParticleSystem::VertexBuffer::InitData(const float* data, unsigned int size)
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}

	void ParticleSystem::VertexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	}

	void ParticleSystem::VertexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
	#pragma endregion

	//********************************************************************************
	// VertexArray 
	//********************************************************************************
	#pragma region VertexArray class
	ParticleSystem::VertexArray::VertexArray()
	{
		GLCall(glGenVertexArrays(1, &m_RendererID));
	}

	ParticleSystem::VertexArray::~VertexArray()
	{
		GLCall(glDeleteVertexArrays(1, &m_RendererID));
	}

	void ParticleSystem::VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
	{
		Bind();
		vb.Bind();
		const auto& elements = layout.GetElements();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); ++i)
		{
			const auto& element = elements[i];
			GLCall(glEnableVertexAttribArray(i + vertexAttribArrayCount));
			//this line below is to avoid a warning related to OpenGL API design flaw for this particular function "glVertexAttribPointer"
			void const* pointer = static_cast<char const*>(0) + offset;
			GLCall(glVertexAttribPointer(i + vertexAttribArrayCount, element.count, element.type, element.normalized, layout.GetStride(), (const void*)pointer));
			offset += element.count * VertexBufferLayout::VertexBufferElement::GetSizeOfType(element.type);
		}
		vertexAttribArrayCount += static_cast<unsigned int>(elements.size());
	}

	void ParticleSystem::VertexArray::Bind() const
	{
		GLCall(glBindVertexArray(m_RendererID));
	}

	void ParticleSystem::VertexArray::Unbind() const
	{
		GLCall(glBindVertexArray(0));
	}
	#pragma endregion

	//********************************************************************************
	// IndexBuffer 
	//********************************************************************************
	#pragma region IndexBuffer class
	ParticleSystem::IndexBuffer::IndexBuffer()
		: m_Count { 0 }
	{
		GLCall(glGenBuffers(1, &m_RendererID));
	}

	ParticleSystem::IndexBuffer::~IndexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}

	void ParticleSystem::IndexBuffer::InitData(const unsigned int* data, unsigned int count)
	{
		m_Count = count;

		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
	}

	void ParticleSystem::IndexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	}

	void ParticleSystem::IndexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
	#pragma endregion

	//********************************************************************************
	// ParticleSystem 
	//********************************************************************************
	#pragma region ParticleSystem class
	unsigned int ParticleSystem::FindInactiveParticle()
	{
		unsigned int i = 0;
		for (; i < maxParticles; ++i)
		{
			if (!m_ParticlePool[i].active)
				return i;
		}
		return maxParticles;
	}

	std::vector<float> ParticleSystem::VerticesDataFormat()
	{
		std::vector<float> coords;
		for (unsigned int i = 0; i < verticesCount; ++i)
		{
			//vertex position
			coords.push_back(vertices[i].x);
			coords.push_back(vertices[i].y);
			coords.push_back(vertices[i].z);

			//color
			coords.push_back(colors[i].r);
			coords.push_back(colors[i].g);
			coords.push_back(colors[i].b);
			coords.push_back(colors[i].a);

			//texture coordinates
			coords.push_back(texCoords[i].x);
			coords.push_back(texCoords[i].y);
		};
		return coords;
	}
	std::vector<float> ParticleSystem::ParticleDataFormat()
	{
		std::vector<float> coords;
		for (auto& particle : m_ParticlePool)
		{
			if (simulationSpace == SimualationSpace::Local)
				particle.transform.parent = &transform;
			if (simulationSpace == SimualationSpace::World)
				particle.transform.parent = nullptr;

			if (!particle.active)
				continue;
			Matrix4x4 pMat = Matrix4x4::Transpose(particle.transform.LocalToWorldMatrix());

			//vertex position
			for (unsigned int j = 0; j < 16; ++j)
			{
				coords.push_back(pMat.m[j]);
			}
		}
		return coords;
	}

	void ParticleSystem::AssertProperties()
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
	bool ParticleSystem::SortParticles(Particle i, Particle j)
	{
		return
			!i.active || (i.lifeTimer > j.lifeTimer);
	}

	Vector3 translations[100];

	std::vector<float> Translations()
	{
		std::vector<float> coords;
		for (unsigned int i = 0; i < 100; ++i)
		{
			//vertex position
			coords.push_back(translations[i].x);
			coords.push_back(translations[i].y);
			coords.push_back(translations[i].z);
		};
		return coords;
	}

	ParticleSystem::ParticleSystem() : texture { new Texture2D("DefaultParticle.png") }
	{
		m_ParticlePool.resize(maxParticles);

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

		int index = 0;
		for (int y = 0; y < 10; ++y)
		{
			for (int x = 0; x < 10; ++x)
			{
				translations[index].x = x * 2.5f;
				translations[index].y = y * 2.5f;
				translations[index].z = 0;
				++index;
			}
		}

		vao.Bind();
		vbo.InitData(VerticesDataFormat().data(), static_cast<unsigned int>(vertices.size() * 9 * sizeof(float)));
		//instancedVbo.InitData(Translations().data(), static_cast<unsigned int>(100 * 3 * sizeof(float)));
		instancedVbo.InitData(ParticleDataFormat().data(), static_cast<unsigned int>(m_activeParticles * 16 * sizeof(float)));
		ibo.InitData(indices.data(), 6);

		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(4);
		layout.Push<float>(2);
		vao.AddBuffer(vbo, layout);
		layout.Clear();
		//layout.Push<float>(3);
		layout.Push<float>(4);
		layout.Push<float>(4);
		layout.Push<float>(4);
		layout.Push<float>(4);
		vao.AddBuffer(instancedVbo, layout);

		ibo.Bind();

		vao.Unbind();
		vbo.Unbind();
		instancedVbo.Unbind();
		ibo.Unbind();

		shader = new Shader("Shaders/DefaultParticle");
		for (unsigned int i = 0; i < 100; i++)
		{
			shader->SetUniformVector3f(("offsets[" + std::to_string(i) + "]"), translations[i]);
		}
		//Emit(100);
	}
	ParticleSystem::~ParticleSystem()
	{

	}
	void ParticleSystem::Update()
	{
		durationTimer += 0.001f;

		while (durationTimer > 1.0f / rateOverTime)
		{
			durationTimer -= 1.0f / rateOverTime;
			Emit(1);
		}

		int index = 0;
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.active)
			{
				++index;
				continue;
			}

			if (particle.lifeTimer >= particle.lifeTime)
			{
				particle.active = false;
				--m_activeParticles;
				++index;
				continue;
			}

			particle.lifeTimer += 0.001f;
			particle.transform.position += particle.velocity * 0.001f;

			particle.transform.rotation = Quaternion::AngleAxis(particle.lifeTimer * index / 50, Vector3::forward());

			++index;
		}
		//std::sort(m_ParticlePool.begin(), m_ParticlePool.end(), SortParticles);
	}
	void ParticleSystem::Render(Camera& camera)
	{
		Matrix4x4 proj = camera.GetProjectionMatrix();
		Matrix4x4 view = camera.GetViewMatrix();
		Matrix4x4 model = transform.LocalToWorldMatrix();

		if (texture)
			texture->Bind(0);

		shader->SetUniform1i("_MainTex", 0);

		shader->SetUniformMatrix4f("_M", model);
		shader->SetUniformMatrix4f("_V", view);
		shader->SetUniformMatrix4f("_P", proj);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		ibo.InitData(indices.data(), static_cast<unsigned int>(indices.size()));
		ibo.Bind();

		vbo.InitData(VerticesDataFormat().data(), static_cast<unsigned int>(vertices.size() * 9 * sizeof(float)));
		vbo.Bind();

		vbo.Unbind();

		//instancedVbo.InitData(Translations().data(), static_cast<unsigned int>(100 * 3 * sizeof(float)));
		instancedVbo.InitData(ParticleDataFormat().data(), static_cast<unsigned int>(m_activeParticles * 16 * sizeof(float)));
		instancedVbo.Bind();

		instancedVbo.Unbind();

		vao.Bind();

		GLCall(glVertexAttribDivisor(0, 0));
		GLCall(glVertexAttribDivisor(1, 0));
		GLCall(glVertexAttribDivisor(2, 0));
		GLCall(glVertexAttribDivisor(3, 1));
		GLCall(glVertexAttribDivisor(4, 1));
		GLCall(glVertexAttribDivisor(5, 1));
		GLCall(glVertexAttribDivisor(6, 1));
		GLCall(glDrawElementsInstanced(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr, m_activeParticles));

		vao.Unbind();
	}

	void ParticleSystem::Emit(unsigned int count)
	{
		for (; count > 0; --count)
		{
			if (m_activeParticles >= maxParticles ||
				FindInactiveParticle() >= m_ParticlePool.size())
				break;
			Particle& particle = m_ParticlePool[FindInactiveParticle()];
			++m_activeParticles;
			particle.active = true;
			particle.transform.position = Vector3();
			particle.transform.rotation = Quaternion::identity;

			// Velocity
			particle.velocity = Vector3::up();
			particle.lifeTime = startLifetime[0];
			particle.lifeTimer = 0;
		}
	}
	unsigned int ParticleSystem::GetActiveParticleCount()
	{
		return m_activeParticles;
	}

	#pragma endregion
}
