#pragma once
#include "DEpch.h"

namespace DeltaEngine
{
  struct State
  {
    struct Parameter
    {
      bool boolValue {false};
      float floatValue { 0.0f };
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

    Parameters parameters{};

    State() = default;

    bool GetBool(std::string paramName);
    bool SetBool(std::string paramName, bool value);

    float GetFloat(std::string paramName);
    float SetFloat(std::string paramName, float value);
  };
}
