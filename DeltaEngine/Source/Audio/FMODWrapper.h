/**********************************************************************************
* \file   FMODWrapper.h
* \brief  This file contains the definition of the engine's FMOD wrapper
*
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include <fmod_studio.hpp>
#include <string>
#include <unordered_map>

namespace DeltaEngine
{
  struct FMODWrapper
  {
    using SoundMap = std::unordered_map<std::string, FMOD::Sound*>;
    using ChannelMap = std::unordered_map<size_t, FMOD::Channel*>;
    using EventMap = std::unordered_map<size_t, FMOD::Studio::EventInstance*>;
    using BankMap = std::unordered_map<std::string, FMOD::Studio::Bank*>;

    // Maps
    SoundMap sounds;
    ChannelMap channels;
    EventMap events;
    BankMap banks;

    // Systems
    FMOD::System* pSystem{nullptr};
    FMOD::Studio::System* pStudioSystem{nullptr};

    size_t nextEventID{0};
    size_t nextChannelID{0};

    FMODWrapper();
    ~FMODWrapper();
    void Update();

    static bool ErrorChecker(FMOD_RESULT result);
  };
}
