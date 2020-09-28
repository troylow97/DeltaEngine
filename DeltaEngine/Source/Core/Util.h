#pragma once

#include "Assets/AssetManager.h"
#include "Render/Font.h"

namespace DeltaEngine
{

template <typename... Ts>
class AssetManager;

using AM = AssetManager<class Font>;

struct DeltaEngineGlobalEnvironment
{
  class GameClock *pClock;
  AM *pmanager;
};

extern DeltaEngineGlobalEnvironment env;

}