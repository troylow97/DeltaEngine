#pragma once

#include "Render/AnimationController.h"
#include "Render/AnimationClip.h"
#include "Render/Texture.h"
#include "Render/Shader.h"
#include "Render/Font.h"

namespace DeltaEngine
{

template <typename... Ts>
class AssetManager;

using AM = AssetManager<class Font, class Texture2D, class Shader, class AnimationClip, class AnimationController>;

class GameClock;

struct DeltaEngineGlobalEnvironment
{
  class GameClock *pClock;
  AM *pManager;
  class ECSModule *pECS;
  class Window *pWin;
};

extern DeltaEngineGlobalEnvironment env;

DeltaEngineGlobalEnvironment& GetEnv();
double DeltaTime();

}