#pragma once

namespace DeltaEngine
{

template <typename... Ts>
class AssetManager;

using AM = AssetManager<class Font, class Texture2D, class Shader>;

struct DeltaEngineGlobalEnvironment
{
  class GameClock *pClock;
  AM *pManager;
};

extern DeltaEngineGlobalEnvironment env;

}