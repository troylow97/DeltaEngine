#pragma once

#include "DEpch.h"
#include "Sprite.h"

struct AnimationEvent
{
	void* event;
	unsigned int time;
};

namespace DeltaEngine
{
	class AnimationClip
	{
		std::string m_Name;
		unsigned int totalFrames;
		unsigned int fps;

		std::vector<Sprite> m_Sprites;
		std::vector<AnimationEvent> m_Events;

		bool loop;
	public:
		AnimationClip(std::string filepath = "Example.clip");
		unsigned int GetTotalFrames() const;
		unsigned int GetFps() const;
		Sprite GetSprite(unsigned int index) const;
		std::string GetName() const;
	private:
		void LoadAnimation(std::string filepath);
		void UpdateAnimation(std::string filepath);
	};
}