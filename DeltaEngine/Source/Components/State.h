/**********************************************************************************
* \file   State.h
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
#pragma once
#include "DEpch.h"

namespace DeltaEngine
{
  struct State
  {
    struct Parameter
    {
      bool boolValue{false};
      float floatValue{0.0f};
    };

    using Parameters = std::unordered_map<std::string, Parameter>;

    Parameters parameters{};

    State() : parameters{}
    {
    };

    State& operator=(const State& rhs)
    {
      parameters.insert(rhs.parameters.begin(), rhs.parameters.end());
      return *this;
    }

    bool GetBool(std::string paramName);
    bool SetBool(std::string paramName, bool value);

    float GetFloat(std::string paramName);
    float SetFloat(std::string paramName, float value);
  };
}
