#pragma once

#include "DEpch.h"
#include "Texture.h"

struct AnimationEvent
{
	void* event;
	unsigned int time;
};

namespace DeltaEngine
{
	class AnimationClip
	{
		std::vector<AnimationEvent> events;
		unsigned int totalFrames;
		float fps;

		Texture2D texture;
		std::vector<unsigned int> textureIndices;

		AnimationClip();
	};
}
