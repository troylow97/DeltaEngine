#pragma once

#include "AnimationClip.h"

namespace DeltaEngine
{
	class Animator
	{
		struct Parameter
		{
			bool boolValue;
			float floatValue;
		};
		struct Transition
		{
			AnimationClip start, end;
			Parameter param;
		};
		std::unordered_map<std::string, Parameter> parameters;
	public:
		float speed;
		AnimationClip animation;
		std::vector<AnimationClip> animations;
		bool playOnAwake;

		bool GetBool(std::string paramName);
		bool SetBool(std::string paramName, bool value);

		float GetFloat(std::string paramName);
		float SetFloat(std::string paramName, float value);
	};
}
