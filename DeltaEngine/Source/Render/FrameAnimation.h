#pragma once

#include "DEpch.h"
#include "SpriteRenderer.h"


namespace DeltaEngine
{
	struct AnimationEvent
	{
		void* event;
		unsigned int time;
	};

	// this class assumes the frames go from left to right, top to bottom
	class FrameAnimation
	{
		float timer = 0;
	public:
		unsigned int frame;
		unsigned int totalFrames;
		unsigned int rows, columns;
		unsigned int fps;
		float speed;
		std::vector<AnimationEvent> events;

		SpriteRenderer* renderer;

		bool playOnAwake;
		bool loop;

		FrameAnimation(unsigned int row = 3, unsigned int col = 2, unsigned int frames = 0);
		~FrameAnimation();
		void Update();
	};
}