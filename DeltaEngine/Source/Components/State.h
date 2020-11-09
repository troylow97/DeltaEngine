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

    Parameters parameters;

    bool GetBool(std::string paramName);
    bool SetBool(std::string paramName, bool value);

    float GetFloat(std::string paramName);
    float SetFloat(std::string paramName, float value);
  };
}
