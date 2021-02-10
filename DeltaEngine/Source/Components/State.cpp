/**********************************************************************************
* \file   State.cpp
* \brief  The file contains BLAHBLAHBLAH
* \author Chin, Clara,   X% Code Contribution
* \author Low, Troy,     X% Code Contribution
* \author Ong, Graeme,   X% Code Contribution
* \author Tan, Tong Wee, X% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "State.h"

namespace DeltaEngine
{
  bool State::GetBool(std::string paramName)
  {
    return parameters[paramName].boolValue;
  }

  bool State::SetBool(std::string paramName, bool value)
  {
    if (!parameters.count(paramName))
      parameters[paramName] = Parameter();
    parameters[paramName].boolValue = value;
    return parameters[paramName].boolValue;
  }

  float State::GetFloat(std::string paramName)
  {
    return parameters[paramName].floatValue;
  }

  float State::SetFloat(std::string paramName, float value)
  {
    if (!parameters.count(paramName))
      parameters[paramName] = Parameter();
    parameters[paramName].floatValue = value;
    return parameters[paramName].floatValue;
  }
}
