#pragma once

#include "AnimationClip.h"
#include "SpriteRenderer.h"

namespace DeltaEngine
{
	class Animator
	{
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
		std::unordered_map<std::string, Parameter> parameters;
		std::vector<std::pair<std::string, std::string>> // name of clips
				transitions;
		std::vector<std::pair<std::string, std::pair<Conditions, float>>> // parameter name/key, and its conditions
				conditions;
		float timer;
		unsigned int frame;
	public:
		float speed;
		SpriteRenderer* renderer;
		AnimationClip* animation;
		bool playOnAwake;

		Animator(std::string filepath = "Player.anim");

		bool GetBool(std::string paramName);
		bool SetBool(std::string paramName, bool value);

		float GetFloat(std::string paramName);
		float SetFloat(std::string paramName, float value);

		void Update();
	private:
		void CheckCondition(std::string paramName);
		void LoadFromFile(std::string filepath);
	};
}
