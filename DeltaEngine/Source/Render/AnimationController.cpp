#include "AnimationController.h"
#include "Core/GlobalStruct.h"
#include "Assets/AssetManager.h"
#include "Core/Debugging/Logger/Log.h"
#include "Core/Math/Math.h"

namespace DeltaEngine
{
AnimationController::AnimationController(std::string filepath)
  : entryAnimation{ nullptr }, m_Name{ std::string(filepath.c_str()) }
{
  LoadFromFile( filepath );
}

AnimationClip *AnimationController::CheckCondition( std::string currentAnim, Parameters &parameters )
{
  for (auto& [StartingState, EndingState, Conditions] : transitions)
  {
    // first check if the start of the transition has the same clip as the currently playing clip
    if (std::strcmp(StartingState.c_str(), currentAnim.c_str()) == 0)
    {
      bool conditionPass = true;
      // next check if all conditions are fulfilled
      for (auto& [ParamName, Condition, Value] : Conditions)
      {
        switch (Condition)
        {
        case Conditions::BoolEqual:
          conditionPass = parameters.at(ParamName).boolValue == (Value != 0);
          break;
        case Conditions::Equal:
          conditionPass = parameters.at(ParamName).floatValue == Value;
          break;
        case Conditions::NotEqual:
          conditionPass = parameters.at(ParamName).floatValue != Value;
          break;
        case Conditions::Greater:
          conditionPass = parameters.at(ParamName).floatValue > Value;
          break;
        case Conditions::Less:
          conditionPass = parameters.at(ParamName).floatValue < Value;
          break;
        }
        if (!conditionPass)
          break;
      }
      // change the clip
      if (conditionPass)
        return GetEnv().pManager->Get<AnimationClip>(EndingState);
    }
  }
  return nullptr;
}

void AnimationController::LoadFromFile( std::string filepath )
{
  std::ifstream file;
  DeltaEngine_CORE_TRACE( "Loading animator \"{}\"...", filepath.c_str() );
  file.open( ( filepath ).c_str() );

  std::string str, defaultClip;

  if ( file.is_open() )
  {
    file >> str >> defaultClip;
    entryAnimation = GetEnv().pManager->Get<AnimationClip>( defaultClip );
    file >> str;
    while ( file.good() ) // parameters
    {
      Parameter newParam;
      file >> str;
      if ( str[0] == '%' )
        break;
      file >> str >> newParam.boolValue >> newParam.floatValue;
      startingParameters.insert( std::pair<std::string, Parameter>( str, newParam ) );
    }
    while ( file.good() ) // transitions and conditions
    {
      std::string paramName, condition;
      float value = 0;
      Conditions con;
      std::string startClip, endClip;

      file >> str >> startClip;
      file >> str >> endClip;

      transitions.push_back({ startClip, endClip, Condition() });

      while ( ( file >> str ), std::strcmp( str.c_str(), "condition" ) == 0 )
      {
        file >> paramName >> condition >> value;
        switch ( condition[0] )
        {
          case '?':
            con = Conditions::BoolEqual;
            break;
          case '=':
            con = Conditions::Equal;
            break;
          case '!':
            con = Conditions::NotEqual;
            break;
          case '>':
            con = Conditions::Greater;
            break;
          case '<':
            con = Conditions::Less;
            break;
        }
        std::get<2>(transitions.back()).push_back({ paramName , con, value });
      }
    }
    file.close();
  }
  else
  {
    DeltaEngine_CORE_WARN( "Animator file \"{}\" doesn't exist", filepath.c_str() );
  }
}
}
