#pragma once
#include "DEpch.h"
#include "DE_API.h"

namespace DeltaEngine
{
  struct DE_API State
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
    using Parameters = std::unordered_map<std::string, Parameter>;
    using Transition = std::pair<std::string, std::string>;
    using Condition = std::vector<std::pair<std::string, std::pair<Conditions, float>>>;

    Parameters parameters;

    std::vector<Transition> // possible animation transitions
      transitions;

    std::vector<Condition> // their conditions
      conditions;

    bool GetBool(std::string paramName);
    bool SetBool(std::string paramName, bool value);

    float GetFloat(std::string paramName);
    float SetFloat(std::string paramName, float value);
  };
}
