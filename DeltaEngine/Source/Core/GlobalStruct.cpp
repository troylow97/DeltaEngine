#include "GlobalStruct.h"
#include "GameClock/EngineClock.h"

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
