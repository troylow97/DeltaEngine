#pragma once
#include "DEpch.h"
#include "Math/Vector.h"
#include "Math/Color.h"

namespace DeltaEngine
{
	class ParticleSystem
	{
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
		float startLifetime;
		Vector3 startVelocity[2];
		float startSize[2];
		float startRotation[2];

		float gravityModifier;
		bool playOnAwake;
		int maxParticles;

		//emission
		int rateOverTime;
		std::vector<Burst> bursts;

		//shape
		int shape = 0;

		//member functions

	};
}
