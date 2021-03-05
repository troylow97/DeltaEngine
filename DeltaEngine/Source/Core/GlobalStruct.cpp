/**********************************************************************************
* \file   GlobalStruct.cpp
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
#include "GlobalStruct.h"
#include "GameClock/EngineClock.h"

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

  double FixedDeltaTime()
  {
    return env.pClock->FixedDeltaTime();
  }

  double FixedUnscaledDeltaTime()
  {
    return env.pClock->FixedUnscaledDeltaTime();
  }
}
