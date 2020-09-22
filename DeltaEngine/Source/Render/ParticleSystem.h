#pragma once

#include "DEpch.h"
#include <GL/glew.h>
#include "Math/Vector.h"
#include "Math/Color.h"

namespace DeltaEngine
{
	class ParticleSystem
	{
		struct Particle
		{
			float lifeTime = 1.0f;
			float lifeTimer = 0.0f;

			Vector2 position;
			Vector2 velocity;
			Color color;
			float rotation;
			float size;
		};
		std::vector<Particle> m_ParticlePool;
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

		//properties
		float duration;
		bool looping;
		bool prewarm;
		float startDelay;
		float startLifetime[2];
		Vector3 startVelocity[2];
		Color startColor[2];
		float startRotation[2];
		float startSize[2];

		float gravityModifier;
		bool playOnAwake;
		int maxParticles;

		//emission
		int rateOverTime;
		std::vector<Burst> bursts;

		//shape
		int shape = 0;

		//member functions
		void Render();
	};
}
