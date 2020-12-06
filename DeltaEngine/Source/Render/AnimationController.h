#pragma once

#include "AnimationClip.h"
#include "Components/State.h"
#include "Assets/AssetKey.h"

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

    std::vector<std::pair<std::string, Parameter>> startingParameters;
    // possible animation transition
    std::vector<Transition> transitions; // start state, end state, parameter name, parameter condition type, parameter condition value

    
    std::vector<std::pair<AssetKey, Vector2>> editorPositions;

    AnimationClip* entryAnimation;
    std::string m_Name;

    AnimationController(std::string filepath = "Dave.anim");

    AnimationClip* CheckCondition(std::string currentAnim, Parameters& parameters);

    Vector2 EditionPositionAt(AssetKey key);
    void AddNewTransition(std::string, std::string);

    static void CreateNew(AnimationClip* clip, std::string filepath = "New.anim");
    void LoadFromFile();
    void SaveToFile();
  };
}
