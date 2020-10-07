#include "AnimationController.h"
#include "Core/GlobalStruct.h"
#include "Assets/AssetManager.h"
#include "Core/Debugging/Logger/Log.h"
#include "Core/Math/Math.h"

namespace DeltaEngine
{
	AnimationController::AnimationController(std::string filepath)
		: entryAnimation{ nullptr }
	{
		LoadFromFile(filepath);
	}

	AnimationClip* AnimationController::CheckCondition(std::string currentAnim, Parameters& parameters)
	{
		//loop through all possible transitions
		for (size_t i = 0; i < transitions.size(); ++i)
		{
			// first check if the start of the transition has the same clip as the currently playing clip
			if (std::strcmp(transitions[i].first.c_str(), currentAnim.c_str()) == 0)
			{
				bool conditionPass = true;
				// next check if all conditions are fulfilled
				for (size_t j = 0; j < conditions[i].size(); ++j)
				{
					switch (conditions[i][j].second.first)
					{
					case Conditions::BoolEqual:
						conditionPass = parameters.at(conditions[i][j].first).boolValue == (conditions[i][j].second.second != 0);
						break;
					case Conditions::Equal:
						conditionPass = parameters.at(conditions[i][j].first).floatValue == conditions[i][j].second.second;
						break;
					case Conditions::NotEqual:
						conditionPass = parameters.at(conditions[i][j].first).floatValue != conditions[i][j].second.second;
						break;
					case Conditions::Greater:
						conditionPass = parameters.at(conditions[i][j].first).floatValue > conditions[i][j].second.second;
						break;
					case Conditions::Less:
						conditionPass = parameters.at(conditions[i][j].first).floatValue < conditions[i][j].second.second;
						break;
					}
					if (!conditionPass)
						break;
				}
				// change the clip
				if (conditionPass)
					return GetEnv().pManager->get<AnimationClip>(transitions[i].second);
			}
		}
		return nullptr;
	}

	void AnimationController::LoadFromFile(std::string filepath)
	{
		std::ifstream file;
		DeltaEngine_CORE_TRACE("Loading animator \"{}\"...", filepath.c_str());
		file.open((filepath).c_str());

		std::string str, defaultClip;

		if (file.is_open())
		{
			file >> str >> defaultClip;
			entryAnimation = GetEnv().pManager->get<AnimationClip>(defaultClip);
			file >> str;
			while (file.good()) // parameters
			{
				Parameter newParam;
				file >> str;
				if (str[0] == '%')
					break;
				file >> str >> newParam.boolValue >> newParam.floatValue;
				startingParameters.insert(std::pair<std::string, Parameter>(str, newParam));
			}
			while (file.good()) // transitions and conditions
			{
				std::string paramName, condition;
				float value = 0;
				Conditions con;
				std::string startClip, endClip;

				file >> str >> startClip;
				file >> str >> endClip;

				transitions.push_back({ startClip, endClip });
				conditions.push_back(Condition());

				while ((file >> str), std::strcmp(str.c_str(), "condition") == 0)
				{
					file >> paramName >> condition >> value;
					switch (condition[0])
					{
					case '?':
						con = Conditions::BoolEqual;
						break;
					case '=':
						con = Conditions::Equal;
						break;
					case '!':
						con = Conditions::NotEqual;
						break;
					case '>':
						con = Conditions::Greater;
						break;
					case '<':
						con = Conditions::Less;
						break;
					}
					conditions.back().push_back(
						{ paramName, { con, value } }
					);
				}
			}
			file.close();
		}
		else
		{
			DeltaEngine_CORE_WARN("Animator file \"{}\" doesn't exist", filepath.c_str());
		}
	}
}
