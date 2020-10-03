#pragma once

#include "AnimationClip.h"

namespace DeltaEngine
{
	class Animator
	{
		struct AnimationTransition
		{

		};
	public:
		float speed;
		AnimationClip animation;
		std::vector<AnimationClip> animations;
	};
}
