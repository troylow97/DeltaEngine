#include "Animator.h"
#include "Core/GlobalStruct.h"
#include "Assets/AssetManager.h"
#include "Core/Debugging/Logger/Log.h"

namespace DeltaEngine
{
	Animator::Animator(std::string filepath)
		: timer{ 0 }, frame{ 0 }, speed{ 1 }, renderer{ nullptr }, animation{ nullptr }, playOnAwake{ false }
	{
		LoadFromFile(filepath);
	}

	bool Animator::GetBool(std::string paramName)
	{
		return parameters.at(paramName).boolValue;
	}

	bool Animator::SetBool(std::string paramName, bool value)
	{
		parameters[paramName].boolValue = value;
		CheckCondition(paramName);
		return parameters.at(paramName).boolValue;
	}

	float Animator::GetFloat(std::string paramName)
	{
		return parameters.at(paramName).floatValue;
	}

	float Animator::SetFloat(std::string paramName, float value)
	{
		parameters[paramName].floatValue = value;
		CheckCondition(paramName);
		return parameters.at(paramName).floatValue;
	}

	void Animator::Update()
	{
		if (!renderer || !animation)
			return;

		timer += static_cast<float>(DeltaTime());
		if (timer > animation->GetTotalFrames() / animation->GetFps())
			timer -= animation->GetTotalFrames() / animation->GetFps();

		frame = (unsigned int)(timer * animation->GetFps());

		//renderer->sprite = animation->GetSprite(frame);
	}

	void Animator::CheckCondition(std::string paramName)
	{
		for (size_t i = 0; i < transitions.size(); ++i)
		{
			if (std::strcmp(conditions[i].first.c_str(), paramName.c_str()) == 0 &&
				std::strcmp(transitions[i].first.c_str(), animation->GetName().c_str()) == 0)
			{
				bool switchClip = false;
				switch (conditions[i].second.first)
				{
				case Conditions::BoolEqual:
					switchClip = parameters.at(paramName).boolValue == (conditions[i].second.second != 0);
					break;
				case Conditions::Equal:
					switchClip = parameters.at(paramName).floatValue == conditions[i].second.second;
					break;
				case Conditions::NotEqual:
					switchClip = parameters.at(paramName).floatValue != conditions[i].second.second;
					break;
				case Conditions::Greater:
					switchClip = parameters.at(paramName).floatValue > conditions[i].second.second;
					break;
				case Conditions::Less:
					switchClip = parameters.at(paramName).floatValue < conditions[i].second.second;
					break;
				}
				if (switchClip)
				{
					animation = GetEnv().pManager->get<AnimationClip>(transitions[i].second);
					timer = 0;
				}
			}
		}
	}

	void Animator::LoadFromFile(std::string filepath)
	{
		std::ifstream file;
		DeltaEngine_CORE_TRACE("Loading animator \"{}\"...", filepath.c_str());
		file.open((filepath).c_str());

		std::string str, defaultClip;

		if (file.is_open())
		{
			file >> str >> defaultClip;
			animation = GetEnv().pManager->get<AnimationClip>(defaultClip);
			std::cout << defaultClip << std::endl;
			file >> str;
			while (file.good()) // parameters
			{
				Parameter newParam;
				file >> str;
				if (str[0] == '%')
					break;
				file >> str >> newParam.boolValue >> newParam.floatValue;
				parameters.insert(std::pair<std::string, Parameter>(str, newParam));
			}
			while (file.good()) // transitions and conditions
			{
				std::string paramName, condition;
				float value;
				Conditions con;
				std::string startClip, endClip;
				file >> str >> paramName >> condition >> value;
				switch (condition[0])
				{
				case '?':
					con = Conditions::BoolEqual;
					break;
				case '>':
					con = Conditions::BoolEqual;
					break;
				case '<':
					con = Conditions::BoolEqual;
					break;
				case '=':
					con = Conditions::BoolEqual;
					break;
				case '!':
					con = Conditions::BoolEqual;
					break;
				}
				file >> str >> startClip;
				file >> str >> endClip;

				transitions.push_back(
					{ startClip, endClip });
				conditions.push_back(
					{ paramName, { con, value } }
				);
			}
			file.close();
		}
		else
		{
			DeltaEngine_CORE_WARN("Animator file \"{}\" doesn't exist", filepath.c_str());
		}
	}
}
