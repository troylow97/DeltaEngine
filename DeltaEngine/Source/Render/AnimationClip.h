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
		unsigned int fps;

		Texture2D texture;
		std::vector<unsigned int> textureIndices;

		bool loop;
	public:
		AnimationClip(std::string filepath = "Example.anim");
		void LoadAnimation();
	};
}
