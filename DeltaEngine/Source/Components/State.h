#pragma once
#include "DEpch.h"

namespace DeltaEngine
{
  struct State
  {
    struct Parameter
    {
      bool boolValue;
      float floatValue;
    };
    using Parameters = std::unordered_map<std::string, Parameter>;

    Parameters parameters{};

    std::vector<Transition> // possible animation transitions
      transitions{};

    std::vector<Condition> // their conditions
      conditions{};

    State() = default;

    bool GetBool(std::string paramName);
    bool SetBool(std::string paramName, bool value);

    float GetFloat(std::string paramName);
    float SetFloat(std::string paramName, float value);
  };
}
