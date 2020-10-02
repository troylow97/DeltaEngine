#include "Animator.h"

namespace DeltaEngine
{
	bool Animator::GetBool(std::string paramName)
	{
		return parameters.at(paramName).boolValue;
	}
	bool Animator::SetBool(std::string paramName, bool value)
	{
		parameters[paramName].boolValue = value;
		return parameters.at(paramName).boolValue;
	}

	float Animator::GetFloat(std::string paramName)
	{
		return parameters.at(paramName).floatValue;
	}
	float Animator::SetFloat(std::string paramName, float value)
	{
		parameters[paramName].floatValue = value;
		return parameters.at(paramName).floatValue;
	}
}
