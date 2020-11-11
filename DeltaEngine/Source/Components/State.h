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
    using Parameters = std::unordered_map<std::string, Parameter>;

    Parameters parameters{};

    State() : parameters{} {};

    State& operator= (const State& rhs)
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
