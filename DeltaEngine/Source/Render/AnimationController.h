#pragma once

#include "AnimationClip.h"
#include "Components/State.h"

namespace DeltaEngine
{
class AnimationController
{
public:
  using Parameter = State::Parameter;
  using Parameters = State::Parameters;
  using Transition = State::Transition;
  using Conditions = State::Conditions;
  using Condition = State::Condition;

  Parameters startingParameters;
  std::vector<Transition> // possible animation transitions
    transitions;
  std::vector<Condition> // their conditions
    conditions;
  AnimationClip *entryAnimation;

  AnimationController( std::string filepath = "Player.anim" );

  AnimationClip *CheckCondition( std::string currentAnim, Parameters &parameters );
private:
  void LoadFromFile( std::string filepath );
};
}
