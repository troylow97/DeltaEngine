#pragma once

namespace DeltaEngine
{

template <typename... Ts>
class AssetManager;

using AM = AssetManager<class Font, class Texture2D, class Shader>;

class GameClock;

struct DeltaEngineGlobalEnvironment
{
  class GameClock *pClock;
  AM *pManager;
  class ECSModule *pECS;
};

extern DeltaEngineGlobalEnvironment env;

}