#pragma once

#include "AnimationClip.h"
#include "SpriteRenderer.h"

namespace DeltaEngine
{
	class AnimationController
	{
	public:
		struct Parameter
		{
			bool boolValue;
			float floatValue;
		};
		enum class Conditions
		{
			BoolEqual,
			Equal,
			NotEqual,
			Greater,
			Less,
		};
		using Parameters = std::unordered_map<std::string, Parameter>;
		using Transition = std::pair<std::string, std::string>;
		using Condition = std::vector<std::pair<std::string, std::pair<Conditions, float>>>;

		Parameters startingParameters;
		std::vector<Transition> // possible animation transitions
			transitions;
		std::vector<Condition> // their conditions
			conditions;
		AnimationClip* entryAnimation;

		AnimationController(std::string filepath = "Player.anim");

		AnimationClip* CheckCondition(std::string currentAnim, Parameters& parameters);
	private:
		void LoadFromFile(std::string filepath);
	};
}
