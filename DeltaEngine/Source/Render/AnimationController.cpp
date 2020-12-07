#include "AnimationController.h"
#include "Core/GlobalStruct.h"
#include "Assets/AssetManager.h"
#include "Core/Debugging/Logger/Log.h"
#include "Core/Math/Math.h"

namespace DeltaEngine
{
  AnimationController::AnimationController(std::string filepath)
    : entryAnimation{""}, m_Name{std::string(filepath.c_str())}
  {
    LoadFromFile();
  }

  AnimationClip* AnimationController::CheckCondition(std::string currentAnim, Parameters& parameters)
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
          if (strcmp(EndingState.c_str(), "Exit"))
            return GetEnv().pManager->Get<AnimationClip>(EndingState);
          else
            return GetEnv().pManager->Get<AnimationClip>(entryAnimation);
      }
    }
    return nullptr;
  }
  Vector2 AnimationController::EditionPositionAt(AssetKey key)
  {
    Vector2 result = Vector2();
    std::for_each(editorPositions.begin(), editorPositions.end(), [&result, &key](std::pair<AssetKey, Vector2>& pair)
      {
        if (pair.first == key)
          result = pair.second;
      });
    return result;
  }
  void AnimationController::AddNewTransition(std::string start, std::string end)
  {
    bool exists = false;
    for (auto& t : transitions)
    {
      if (!strcmp(std::get<0>(t).c_str(), start.c_str()) &&
        !strcmp(std::get<1>(t).c_str(), end.c_str()))
      {
        exists = true;
        break;
      }
    }
    if (!exists)
    {
      transitions.push_back(
        { std::string(start.c_str()), std::string(end.c_str()), Condition() });
    }
  }
  void AnimationController::CreateNew(std::string filepath)
  {
    AnimationController c = AnimationController(filepath);
    c.editorPositions.resize(2);
    c.editorPositions[0] = {"Entry", Vector2(0, 0)};
    c.editorPositions[1] = {"Exit", Vector2(200, 200)};

    std::ofstream file;
    DeltaEngine_CORE_TRACE("Saving animator \"{}\"...", filepath.c_str());
    file.open(filepath.c_str());

    std::string str;

    if (file.is_open())
    {
      file << "%Entry" << std::endl << std::endl;
      file << "%Parameters:" << std::endl << std::endl;

      file << "%EditorPositions:" << std::endl << std::endl;
      for (auto& [ClipKey, Pos] : c.editorPositions)
        file << "pos" << std::endl
        << ClipKey.Key() << " "
        << Pos.x << " "
        << Pos.y << std::endl << std::endl;
      file << "%Transitions:" << std::endl << std::endl;
      file << "%endfile" << std::endl;
      file.close();
    }
    else
    {
      DeltaEngine_CORE_WARN("Failed to create Animator file \"{}\"", filepath.c_str());
    }

  }
  void AnimationController::LoadFromFile()
  {
    std::ifstream file;
    DeltaEngine_CORE_TRACE("Loading animator \"{}\"...", m_Name.c_str());
    file.open(m_Name.c_str());

    std::string str;

    if (file.is_open())
    {
      startingParameters.clear();
      editorPositions.clear();
      transitions.clear();
      file >> str;
      file >> str;
      if (str[0] != '%')
      {
        file >> entryAnimation;
        file >> str;
      }
      while (file.good()) // parameters
      {
        Parameter newParam;
        file >> str;
        if (str[0] == '%')
          break;
        file >> str >> newParam.boolValue >> newParam.floatValue;
        startingParameters.push_back(std::pair<std::string, Parameter>(str, newParam));
      }
      while (file.good()) // editor positions
      {
        Vector2 pos;
        file >> str;
        if (str[0] == '%')
          break;
        file >> str >> pos.x >> pos.y;
        editorPositions.push_back(std::pair<AssetKey, Vector2>(str, pos));
      }
      while (file.good()) // transitions and conditions
      {
        std::string paramName, condition;
        float value = 0;
        Conditions con;
        std::string startClip, endClip;

        file >> str;
        if (str[0] == '%')
          break;
        file >> str >> startClip;
        file >> str >> endClip;

        transitions.push_back({startClip, endClip, Condition()});

        while ((file >> str), std::strcmp(str.c_str(), "condition") == 0)
        {
          file >> paramName >> condition >> value;
          switch (condition[0])
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
          std::get<2>(transitions.back()).push_back({paramName, con, value});
        }
      }
      file.close();
      //for (auto& [s,e,c] : transitions)
      //{
      //  std::cerr << s << "->" << e << std::endl;
      //}
    }
    else
    {
      DeltaEngine_CORE_WARN("Animator file \"{}\" doesn't exist", m_Name.c_str());
    }
  }
  void AnimationController::SaveToFile()
  {
    // remove duplicates
    std::vector<std::pair<std::string, Parameter>> newParameters;
    {
      for (auto& [paramName, val] : startingParameters)
      {
        bool dup = false;
        for (auto& [cmp, cval] : newParameters)
          if (!strcmp(paramName.c_str(), cmp.c_str()))
          {
            dup = true;
            break;
          }
        if (!dup)
          newParameters.push_back({ std::string(paramName.c_str()), val });
      }
    }
    std::vector<std::pair<AssetKey, Vector2>> newPositions;
    {
      for (auto& [assetName, val] : editorPositions)
      {
        bool dup = false;
        for (auto& [cmp, cval] : newPositions)
          if (!strcmp(assetName.Key().c_str(), cmp.Key().c_str()))
          {
            dup = true;
            break;
          }
        if (!dup)
          newPositions.push_back({ assetName, val });
      }
    }

    std::ofstream file;
    DeltaEngine_CORE_TRACE("Saving animator \"{}\"...", m_Name.c_str());
    file.open(m_Name.c_str());

    std::string str;

    if (file.is_open())
    {
      file << "%Entry:" << std::endl << std::endl;
      file << "entry " << std::endl << entryAnimation << std::endl << std::endl;
      file << "%Parameters:" << std::endl << std::endl;
      for (auto& [ParamName, Value] : newParameters)
        file << "param" << std::endl
          << ParamName << " "
          << Value.boolValue << " "
          << Value.floatValue << std::endl << std::endl;

      file << "%EditorPositions:" << std::endl << std::endl;
      for (auto& [ClipKey, Pos] : newPositions)
        file << "pos" << std::endl
          << ClipKey.Key() << " "
          << Pos.x << " "
          << Pos.y << std::endl << std::endl;
      file << "%Transitions:" << std::endl << std::endl;

      for (auto& [StartClip, EndClip, Conditions] : transitions)
      {
        file << "transition " << std::endl;
        file << "start " << StartClip << std::endl;
        file << "end " << EndClip << std::endl;
        for (auto& [ParamName, ConditionType, ConditionValue] : Conditions)
        {
          char ct = '?';
          switch (ConditionType)
          {
          case Conditions::BoolEqual:
            ct = '?';
            break;
          case Conditions::Equal:
            ct = '=';
            break;
          case Conditions::NotEqual:
            ct = '!';
            break;
          case Conditions::Greater:
            ct = '>';
            break;
          case Conditions::Less:
            ct = '<';
            break;
          }

          file << "condition " <<
            ParamName << ' ' << ct << ' ' << ConditionValue << std::endl;
        }
        file << "endTransition" << std::endl << std::endl;
      }
      file << "%endfile" << std::endl;
      file.close();
    }
    else
    {
      DeltaEngine_CORE_WARN("Animator file \"{}\" failed to save!", m_Name.c_str());
    }
  }
}
