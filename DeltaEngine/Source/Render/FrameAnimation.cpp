#include "FrameAnimation.h"

namespace DeltaEngine
{
	FrameAnimation::FrameAnimation(unsigned int row, unsigned int col, unsigned int frames)
		: timer{ 0 }, frame{ 0 }, totalFrames{ frames == 0 ? row * col : frames }, rows{ row }, columns{ col },
		fps{ 12 }, speed{ 1 }, renderer{ nullptr }, playOnAwake{ true }, loop{ true }
	{

	}

	FrameAnimation::~FrameAnimation()
	{

	}
	
	void FrameAnimation::Update(double frameTime)
	{
		timer += frameTime;
		if (timer > totalFrames / fps)
			timer -= totalFrames / fps;

		frame = (unsigned int)(timer * fps);

		renderer->m_Offset.x = frame % columns * 1.0f / columns;
		renderer->m_Offset.y = frame / columns * 1.0f / rows;
		
		renderer->m_Tiling.x = 1.0f / columns;
		renderer->m_Tiling.y = 1.0f / rows;
	}
}
