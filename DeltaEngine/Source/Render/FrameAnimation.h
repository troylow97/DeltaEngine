#pragma once

#include "DEpch.h"


namespace DeltaEngine
{
	// this class assumes the frames go from left to right, top to bottom
	struct AnimationEvent
	{
		void* event;
		unsigned int time;
	};

	class FrameAnimation
	{
		float timer = 0;
	public:
		unsigned int frame;
		unsigned int totalFrames;
		unsigned int rows, columns;
		unsigned int fps;
		std::vector<AnimationEvent> events;

		bool playOnAwake;
		bool loop;

		FrameAnimation();
		~FrameAnimation();
		void Update();
	};
}