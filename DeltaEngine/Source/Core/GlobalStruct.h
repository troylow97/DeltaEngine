/**********************************************************************************
* \file   GlobalStruct.h
* \brief  The file contains BLAHBLAHBLAH
* \author Chin, Clara,   X% Code Contribution
* \author Low, Troy,     X% Code Contribution
* \author Ong, Graeme,   X% Code Contribution
* \author Tan, Tong Wee, X% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "Render/AnimationController.h"
#include "Render/AnimationClip.h"
#include "Render/Texture.h"
#include "Render/Shader.h"
#include "Render/Font.h"
#include "Event/Event.h"

namespace DeltaEngine
{
  template <typename... Ts>
  class AssetManager;

  using AM = AssetManager<class Font, class Texture2D, class Shader, class AnimationClip, class AnimationController>;

  class EngineClock;

  struct DeltaEngineGlobalEnvironment
  {
    class EngineClock* pClock;
    AM* pManager;
    EventManager* eventManager;
    class ECSModule* pECS;
    class Window* pWin;
  };

  extern DeltaEngineGlobalEnvironment env;

  DeltaEngineGlobalEnvironment& GetEnv();
  float DeltaTimef();
  double DeltaTime();
  float FixedDeltaTimef();
  double FixedDeltaTime();
  double FixedUnscaledDeltaTime();
}