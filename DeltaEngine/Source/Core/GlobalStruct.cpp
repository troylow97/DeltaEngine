#include "GlobalStruct.h"
#include "GameClock/GameClock.h"

namespace DeltaEngine
{

    DeltaEngineGlobalEnvironment& GetEnv()
    {
        return env;
    }

    double DeltaTime()
    {
        return env.pClock->DeltaTime();
    }

}