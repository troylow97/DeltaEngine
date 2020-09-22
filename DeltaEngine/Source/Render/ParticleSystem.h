#pragma once

#include "DEpch.h"
#include <GL/glew.h>
#include "Math/Vector.h"
#include "Math/Color.h"
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"

namespace DeltaEngine
{
	class ParticleSystem
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
			unsigned int vertexAttribArrayCount = 0;
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
		VertexBuffer vbo, instancedVbo;
		IndexBuffer ibo;

		const unsigned int MAX_VERTICES = 65534;
		const unsigned int MAX_PARTICLES = 1000;

		unsigned int verticesCount = 0;

		std::vector<Vector3> vertices;
		std::vector<Color> colors;
		std::vector<Vector2> texCoords;
		//std::vector<Vector3> normals;
		std::vector<unsigned int> indices;

		std::vector<float> VerticesDataFormat();
		std::vector<float> ParticleDataFormat();
		void AssertProperties();

		struct Particle
		{
			float lifeTime = 1.0f;
			float lifeTimer = 0.0f;

			Transform transform;

			Vector3 velocity;
			Color color;

			bool active = false;
		};
		static bool SortParticles(Particle, Particle);
		std::vector<Particle> m_ParticlePool;
		unsigned int m_activeParticles = 0;

		float durationTimer = 0;

		unsigned int FindInactiveParticle();
	public:
		struct Burst
		{
			float time;
			int count;
		};
		enum Shape
		{
			None, Circle, Line
		};
		enum SimualationSpace
		{
			Local, World,
		};

		Transform transform;

		//properties
		float duration = 5;
		bool looping = true;
		bool prewarm = false;
		float startDelay = 0;
		float startLifetime[2] = { 1.0f, 1.0f };
		Vector3 startVelocity[2] = { Vector3(), Vector3() };
		Color startColor[2] = { Color(), Color() };
		float startRotation[2] = { 0, 0 };
		float startSize[2] = { 1, 1 };
		SimualationSpace simulationSpace = Local;

		bool playOnAwake = true;
		unsigned int maxParticles = 100;

		//emission
		int rateOverTime = 30;
		std::vector<Burst> bursts;

		//shape
		int shape = 0;

		//rendering
		Shader* shader;
		Texture2D* texture;

		//member functions
		ParticleSystem();
		~ParticleSystem();
		void Update();
		void Render(Camera& camera);
		void Emit(unsigned int count);
		unsigned int GetActiveParticleCount();
	};
}
