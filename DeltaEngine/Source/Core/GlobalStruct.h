#pragma once

#include "Render/Animator.h"
#include "Render/AnimationClip.h"
#include "Render/Texture.h"
#include "Render/Shader.h"
#include "Render/Font.h"

namespace DeltaEngine
{

template <typename... Ts>
class AssetManager;

using AM = AssetManager<class Font, class Texture2D, class Shader, class AnimationClip, class Animator>;

class GameClock;

struct DeltaEngineGlobalEnvironment
{
  class GameClock *pClock;
  AM *pManager;
  class ECSModule *pECS;
};

extern DeltaEngineGlobalEnvironment env;

DeltaEngineGlobalEnvironment& GetEnv();
double DeltaTime();

}