#include "FrameAnimation.h"

namespace DeltaEngine
{
	FrameAnimation::FrameAnimation(unsigned int row, unsigned int col, unsigned int frames)
		: timer{ 0 }, frame{ 0 }, totalFrames{ frames == 0 ? row * col : frames }, rows{ row }, columns{ col },
		fps{ 30 }, speed{ 1 }, playOnAwake{ true }, loop{ true },
		renderer{ nullptr }
	{

	}
	FrameAnimation::~FrameAnimation()
	{

	}
	void FrameAnimation::Update()
	{
		timer += 0.001f;
		if (timer > totalFrames / fps)
			timer -= totalFrames / fps;

		frame = (unsigned int)(timer * fps);

		renderer->offset.x = frame % columns * 1.0f / columns;
		renderer->offset.y = frame / columns * 1.0f / rows;

		renderer->tiling.x = 1.0f / columns;
		renderer->tiling.y = 1.0f / rows;
	}

}