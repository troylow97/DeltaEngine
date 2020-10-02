#pragma once

#include "DEpch.h"
#include "SpriteRenderer.h"


namespace DeltaEngine
{
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

		SpriteRenderer* renderer;

		bool playOnAwake;
		bool loop;

		FrameAnimation(unsigned int row = 3, unsigned int col = 2, unsigned int frames = 0);
		~FrameAnimation();
		void Update(double frameTime);
	};
}