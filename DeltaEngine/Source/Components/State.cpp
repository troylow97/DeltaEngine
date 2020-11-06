#include "State.h"

namespace DeltaEngine
{
  bool State::GetBool(std::string paramName)
  {
    return parameters[paramName].boolValue;
  }

  bool State::SetBool(std::string paramName, bool value)
  {
    if (parameters[paramName].boolValue == value)
      return parameters.at(paramName).boolValue;
    parameters[paramName].boolValue = value;
    return parameters[paramName].boolValue;
  }

  float State::GetFloat(std::string paramName)
  {
    return parameters[paramName].floatValue;
  }

  float State::SetFloat(std::string paramName, float value)
  {
    if (parameters[paramName].floatValue == value)
      return parameters.at(paramName).floatValue;
    parameters[paramName].floatValue = value;
    return parameters[paramName].floatValue;
  }
}
