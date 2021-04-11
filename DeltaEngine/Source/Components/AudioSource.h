/**********************************************************************************
* \file   AudioSource.h
* \brief  The audio source component
* \author Tan Tong Wee     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Audio/AudioEngine.h"

namespace DeltaEngine
{
  struct AudioSource
  {
    std::string clip {};
    size_t id {0};
    float volume {1.0f};
    float pitch {0.0f};
    int loop {0};
    bool isLoop {false};
    bool isEvent {false};
    bool is3D { false };
    bool isStart { false };
    bool isPlaying { false };
    bool isPlayed { false };

    ~AudioSource()
    {
      AudioEngine::StopEvent( id );
    }
  };
}