#include "GlobalStruct.h"
#include "GameClock/GameClock.h"

namespace DeltaEngine
{
  DeltaEngineGlobalEnvironment& GetEnv()
  {
    return env;
  }

  double FixedDeltaTime()
  {
    return env.pClock->FixedDeltaTime();
  }
}
