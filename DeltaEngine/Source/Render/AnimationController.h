#pragma once

#include "AnimationClip.h"
#include "Components/State.h"

namespace DeltaEngine
{
  class AnimationController
  {
  public:
    enum class Conditions
    {
      BoolEqual,
      Equal,
      NotEqual,
      Greater,
      Less,
    };

    using Parameter = State::Parameter;
    using Parameters = State::Parameters;
    // actual condition
    using Condition = std::vector<std::tuple<std::string, Conditions, float>>;

    // start state, end state, parameter name, parameter condition type, parameter condition value
    using Transition = std::tuple<std::string, std::string, Condition>;

    Parameters startingParameters;
    std::vector<Transition> // possible animation transitions
    transitions;

    AnimationClip* entryAnimation;
    std::string m_Name;

    AnimationController(std::string filepath = "Dave.anim");

    AnimationClip* CheckCondition(std::string currentAnim, Parameters& parameters);
  private:
    void LoadFromFile(std::string filepath);
  };
}
