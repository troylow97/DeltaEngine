/**********************************************************************************
* \file   FMODWrapper.cpp
* \brief  This file contains the implementation of the engine's FMOD wrapper
*
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "FMODWrapper.h"

#include "Core/Debugging/Logger/Log.h"

namespace DeltaEngine
{
  FMODWrapper::FMODWrapper()
  {
    ErrorChecker(FMOD::Studio::System::create(&pStudioSystem));
    ErrorChecker(pStudioSystem->initialize(64, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, nullptr));
    ErrorChecker(pStudioSystem->getCoreSystem(&pSystem));
  }

  FMODWrapper::~FMODWrapper()
  {
    ErrorChecker(pStudioSystem->unloadAll());
    ErrorChecker(pStudioSystem->release());
  }

  bool FMODWrapper::ErrorChecker(FMOD_RESULT result)
  {
    if (result != FMOD_OK)
    {
      DeltaEngine_CORE_WARN("FMOD Error Code - {}", result);
      return true;
    }
    return false;
  }

  void FMODWrapper::Update()
  {
    // Studio
    if (!events.empty())
      for (auto it = events.begin(); it != events.end();)
      {
        if (!it->second->isValid())
        {
          it = events.erase(it);
          continue;
        }
        FMOD_STUDIO_PLAYBACK_STATE state;
        it->second->getPlaybackState(&state);

        if (state == FMOD_STUDIO_PLAYBACK_STOPPED)
        {
          it->second->release();
          it = events.erase(it);
          continue;
        }
        ++it;
      }

    // Core
    if (!channels.empty())
      for (auto it = channels.begin(); it != channels.end();)
      {
        bool isPlaying{false};
        it->second->isPlaying(&isPlaying);
        if (!isPlaying)
          it = channels.erase(it);
        else
          ++it;
      }

    // System
    ErrorChecker(pStudioSystem->update());
    //ErrorChecker(pSystem->update());
  }
}
