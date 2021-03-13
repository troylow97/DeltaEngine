/**********************************************************************************
* \file   AudioEngine.cpp
* \brief  This file contains the implementation of the Audio Engine that interacts with
*         FMOD through the wrapper
*
*         Audio Engine provides the following functionality:
*         - FMOD Studio
*           - 2D / 3D Events
*           - Parameters
*
*         - FMOD Core
*           - Play Sound 2D / 3D
*           - Global parameters
*
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "AudioEngine.h"
#include "FMODWrapper.h"
#include "Core/TypeAlias.h"
#include "Core/Debugging/Logger/Log.h"
#include "Core/Debugging/Profiler/Profiler.h"

namespace DeltaEngine
{
  FMODWrapper* fmod{nullptr};

  FMOD_VECTOR ToFMODVector(const Vector3 vec)
  {
    return FMOD_VECTOR{vec.x, vec.y, vec.z};
  }

  FMOD_3D_ATTRIBUTES To3DAttributes(const Audio3DAttributes attributes)
  {
    return FMOD_3D_ATTRIBUTES
    {
      ToFMODVector(attributes.pos),
      ToFMODVector(attributes.vel),
      ToFMODVector(attributes.forward),
      ToFMODVector(attributes.up)
    };
  }


  void AudioEngine::Initialize()
  {
    if (!fmod)
    {
      DeltaEngine_CORE_INFO("Initializing FMOD...");
      fmod = new FMODWrapper();
      DeltaEngine_CORE_INFO("Initializing FMOD successful");
    }
  }

  void AudioEngine::Shutdown()
  {
    DeltaEngine_CORE_INFO("Shutting down FMOD...");
    delete fmod;
    DeltaEngine_CORE_INFO("Shutting down FMOD successful");
  }

  void AudioEngine::Update()
  {
    fmod->Update();
    Profiler::Instance().Record("Audio Update");
  }

  // Core
  void AudioEngine::LoadSound(const std::string& name, bool loop, bool stream, bool is3D)
  {
    if (auto result = fmod->sounds.find(name); result == fmod->sounds.end())
    {
      FMOD_MODE mode = FMOD_DEFAULT;
      mode |= loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
      mode |= stream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;
      mode |= is3D ? FMOD_3D : FMOD_2D;

      FMOD::Sound* sound{nullptr};
      FMODWrapper::ErrorChecker(fmod->pSystem->createSound(name.c_str(), mode, nullptr, &sound));
      if (sound)
        fmod->sounds[name] = sound;
    }
  }

  void AudioEngine::UnloadSound(const std::string& name)
  {
    if (auto result = fmod->sounds.find(name); result != fmod->sounds.end())
    {
      FMODWrapper::ErrorChecker(result->second->release());
      fmod->sounds.erase(result);
    }
  }

  AudioEngine::ChannelID AudioEngine::Play(const std::string& name, const float dB, Vector3 pos)
  {
    ChannelID id = fmod->nextChannelID++;
    auto result = fmod->sounds.find(name);
    if ( result == fmod->sounds.end() )
    {
      DeltaEngine_CORE_WARN( "Audio - \"{}\" not found", name);
      return 0;
    }

    FMOD::Channel* channel{nullptr};
    FMODWrapper::ErrorChecker(fmod->pSystem->playSound(result->second, nullptr, true, &channel));
    if (channel)
    {
      FMOD_MODE mode;
      result->second->getMode(&mode);
      if (mode & FMOD_3D)
      {
        auto f_vec = ToFMODVector(pos);
        FMODWrapper::ErrorChecker(channel->set3DAttributes(&f_vec, nullptr));
      }
      FMODWrapper::ErrorChecker(channel->setVolume(dBToVolume(dB)));
      FMODWrapper::ErrorChecker(channel->setPaused(false));
      fmod->channels[id] = channel;
    }
    return id;
  }

  bool AudioEngine::IsChannelPlaying(const ChannelID id)
  {
    bool isPlaying{false};

    if (auto result = fmod->channels.find(id); result != fmod->channels.end())
      result->second->isPlaying(&isPlaying);

    return isPlaying;
  }

  void AudioEngine::SetChannelPause(const ChannelID id, const bool pause)
  {
    if (auto result = fmod->channels.find(id); result != fmod->channels.end())
      FMODWrapper::ErrorChecker(result->second->setPaused(pause));
  }

  void AudioEngine::SetChannelPitch(const ChannelID id, const float pitch)
  {
    if (auto result = fmod->channels.find(id); result != fmod->channels.end())
      FMODWrapper::ErrorChecker(result->second->setPitch(pitch));
  }

  void AudioEngine::SetChannelVolume(const ChannelID id, const float volume)
  {
    if (auto result = fmod->channels.find(id); result != fmod->channels.end())
      FMODWrapper::ErrorChecker(result->second->setVolume(volume));
  }

  void AudioEngine::SetChannelMode(ChannelID id, unsigned mode)
  {
    if (auto result = fmod->channels.find(id); result != fmod->channels.end())
      FMODWrapper::ErrorChecker(result->second->setMode(mode));
  }

  void AudioEngine::SetChannelLoop(ChannelID id)
  {
    if (auto result = fmod->channels.find(id); result != fmod->channels.end())
    {
      FMOD_MODE mode;
      FMODWrapper::ErrorChecker( result->second->getMode( &mode ) );
      SetChannelMode( id, mode | FMOD_LOOP_NORMAL );
    }
  }

  void AudioEngine::SetChannelLoopCount(ChannelID id, int count)
  {
    if (auto result = fmod->channels.find(id); result != fmod->channels.end())
      FMODWrapper::ErrorChecker(result->second->setLoopCount(count));
  }

  void AudioEngine::StopChannel(const ChannelID id)
  {
    if (auto result = fmod->channels.find(id); result != fmod->channels.end())
      FMODWrapper::ErrorChecker(result->second->stop());
  }

  void AudioEngine::StopChannels()
  {
    for (auto& channel : fmod->channels)
      FMODWrapper::ErrorChecker(channel.second->stop());
  }

  void AudioEngine::SetChannel3DPosition(const ChannelID id, const Vector3 vec)
  {
    if (auto result = fmod->channels.find(id); result != fmod->channels.end())
    {
      auto f_vec = ToFMODVector(vec);
      FMODWrapper::ErrorChecker(result->second->set3DAttributes(&f_vec, nullptr));
    }
  }

  unsigned AudioEngine::GetChannelPlaybackPosition(ChannelID id)
  {
    if (auto result = fmod->channels.find(id); result != fmod->channels.end())
    {
      unsigned pos{0};
      FMODWrapper::ErrorChecker(result->second->getPosition(&pos, FMOD_TIMEUNIT_MS));
      return pos;
    }
    return 0;
  }

  unsigned AudioEngine::GetSoundLength(const std::string& name)
  {
    if (auto result = fmod->sounds.find(name); result != fmod->sounds.end())
    {
      unsigned pos{0};
      FMODWrapper::ErrorChecker(result->second->getLength(&pos, FMOD_TIMEUNIT_MS));
      return pos;
    }
    return 0;
  }


  // Studio
  bool AudioEngine::IsLoadedBank(const std::string& name)
  {
    if (auto result = fmod->banks.find(name); result != fmod->banks.end())
      return true;
    return false;
  }


  void AudioEngine::LoadBank(const std::string& name, FMOD_STUDIO_LOAD_BANK_FLAGS flags)
  {
    if (auto result = fmod->banks.find(name); result != fmod->banks.end())
      return;

    FMOD::Studio::Bank* bank{nullptr};
    if (FMODWrapper::ErrorChecker(
      fmod->pStudioSystem->
            loadBankFile(name.c_str(), flags, &bank)))
      return;

    fmod->banks[name] = bank;
  }

  void AudioEngine::UnloadBank(const std::string& name)
  {
    if (auto result = fmod->banks.find(name); result != fmod->banks.end())
    {
      if (FMODWrapper::ErrorChecker(result->second->unload()))
        return;

      fmod->banks.erase(name);
    }
  }

  std::vector<std::string> AudioEngine::EventList(const std::string& name)
  {
    if (auto result = fmod->banks.find(name); result != fmod->banks.end())
    {
      int counter{0};
      FMOD::Studio::EventDescription* desc[256];
      if (FMODWrapper::ErrorChecker(result->second->getEventList(desc, 256, &counter)))
        return {};

      std::vector<std::string> str_vec;
      for (int i = 0; i < counter; i ++)
      {
        char buffer[1024];
        int retrieved{0};
        desc[i]->getPath(buffer, 1024, &retrieved);
        str_vec.push_back(buffer);
      }
      return str_vec;
    }
    return {};
  }

  AudioEngine::EventID AudioEngine::Play3DEvent(const std::string& name, const Audio3DAttributes attributes,
                                                const ParametersMap parameters)
  {
    FMOD::Studio::EventDescription* description = nullptr;
    if (FMODWrapper::ErrorChecker(fmod->pStudioSystem->getEvent(name.c_str(), &description)))
      return 0;

    FMOD::Studio::EventInstance* instance = nullptr;
    if (FMODWrapper::ErrorChecker(description->createInstance(&instance)))
      return 0;

    bool is3D{false};
    description->is3D(&is3D);

    if (!is3D)
      return 0;
    auto attr = To3DAttributes(attributes);
    if (FMODWrapper::ErrorChecker(instance->set3DAttributes(&attr)))
      return 0;

    if (!parameters.empty())
      for (auto [parameter, value] : parameters)
        FMODWrapper::ErrorChecker(instance->setParameterByName(parameter.c_str(), value, false));

    fmod->events.insert({++fmod->nextEventID, instance});
    if (FMODWrapper::ErrorChecker(instance->start()))
      return 0;

    return fmod->nextEventID;
  }


  AudioEngine::EventID AudioEngine::Play2DEvent(const std::string& name, const ParametersMap parameters)
  {
    FMOD::Studio::EventDescription* description = nullptr;
    if (FMODWrapper::ErrorChecker(fmod->pStudioSystem->getEvent(name.c_str(), &description)))
      return 0;

    FMOD::Studio::EventInstance* instance = nullptr;
    if (FMODWrapper::ErrorChecker(description->createInstance(&instance)))
      return 0;

    if (!parameters.empty())
      for (auto [parameter, value] : parameters)
        FMODWrapper::ErrorChecker(instance->setParameterByName(parameter.c_str(), value, false));

    fmod->events.insert({++fmod->nextEventID, instance});
    if (FMODWrapper::ErrorChecker(instance->start()))
      return 0;

    return fmod->nextEventID;
  }

  bool AudioEngine::IsEventPlaying(const EventID id)
  {
    bool isPlaying{false};
    if (auto result = fmod->events.find(id); result != fmod->events.end())
    {
      FMOD_STUDIO_PLAYBACK_STATE state;
      FMODWrapper::ErrorChecker(result->second->getPlaybackState(&state));
      if (state != FMOD_STUDIO_PLAYBACK_STOPPED)
        isPlaying = true;
    }
    return isPlaying;
  }

  void AudioEngine::SetEventPause(const EventID id, const bool pause)
  {
    if (auto result = fmod->events.find(id); result != fmod->events.end())
      FMODWrapper::ErrorChecker(result->second->setPaused(pause));
  }

  void AudioEngine::SetEventPitch(const EventID id, const float pitch)
  {
    if (auto result = fmod->events.find(id); result != fmod->events.end())
      FMODWrapper::ErrorChecker(result->second->setPitch(pitch));
  }

  void AudioEngine::SetEventVolume(const EventID id, const float volume)
  {
    if (auto result = fmod->events.find(id); result != fmod->events.end())
      FMODWrapper::ErrorChecker(result->second->setVolume(volume));
  }

  void AudioEngine::StopEvent(const EventID id, const bool fade)
  {
    if (auto result = fmod->events.find(id); result != fmod->events.end())
    {
      if (fade)
        FMODWrapper::ErrorChecker(result->second->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT));
      else
        FMODWrapper::ErrorChecker(result->second->stop(FMOD_STUDIO_STOP_IMMEDIATE));
    }
  }

  void AudioEngine::SetEvent3DAttribute(const EventID id, const Audio3DAttributes attributes)
  {
    if (auto result = fmod->events.find(id); result != fmod->events.end())
    {
      auto attr = To3DAttributes(attributes);
      FMODWrapper::ErrorChecker(result->second->set3DAttributes(&attr));
    }
  }

  float AudioEngine::GetEventParameterByName(const EventID id, const std::string& parameter)
  {
    float val{0.0f};
    if (auto result = fmod->events.find(id); result != fmod->events.end())
      FMODWrapper::ErrorChecker(result->second->getParameterByName(parameter.c_str(), &val));
    return val;
  }


  void AudioEngine::SetEventParameterByName(const EventID id, const std::string& parameter, const float value)
  {
    if (auto result = fmod->events.find(id); result != fmod->events.end())
      FMODWrapper::ErrorChecker(result->second->setParameterByName(parameter.c_str(), value));
  }

  void AudioEngine::SetBusMute(const std::string& name, const bool mute)
  {
    FMOD::Studio::Bus* bus{nullptr};
    FMODWrapper::ErrorChecker(fmod->pStudioSystem->getBus(name.c_str(), &bus));
    if (bus->isValid())
      FMODWrapper::ErrorChecker(bus->setMute(mute));
  }


  void AudioEngine::SetBusPause(const std::string& name, bool pause)
  {
    FMOD::Studio::Bus* bus{nullptr};
    FMODWrapper::ErrorChecker(fmod->pStudioSystem->getBus(name.c_str(), &bus));
    if (bus->isValid())
      FMODWrapper::ErrorChecker(bus->setPaused(pause));
  }

  void AudioEngine::SetBusVolume(const std::string& name, const float volume)
  {
    FMOD::Studio::Bus* bus{nullptr};
    FMODWrapper::ErrorChecker(fmod->pStudioSystem->getBus(name.c_str(), &bus));
    if (bus->isValid())
      FMODWrapper::ErrorChecker(bus->setVolume(volume));
  }

  void AudioEngine::StopAllBusEvents(const std::string& name, bool fade)
  {
    FMOD::Studio::Bus* bus{nullptr};
    FMODWrapper::ErrorChecker(fmod->pStudioSystem->getBus(name.c_str(), &bus));
    if (bus->isValid())
    {
      if (fade)
        FMODWrapper::ErrorChecker(bus->stopAllEvents(FMOD_STUDIO_STOP_ALLOWFADEOUT));
      else
        FMODWrapper::ErrorChecker(bus->stopAllEvents(FMOD_STUDIO_STOP_IMMEDIATE));
    }
  }

  void AudioEngine::Set3DListenerAttributes(const Audio3DAttributes attributes)
  {
    auto attr = To3DAttributes(attributes);
    FMODWrapper::ErrorChecker(fmod->pStudioSystem->setListenerAttributes(0, &attr));
  }

  float AudioEngine::GetGlobalParameterByName(const std::string& parameter)
  {
    float val{0.0f};
    FMODWrapper::ErrorChecker(fmod->pStudioSystem->getParameterByName(parameter.c_str(), &val));
    return val;
  }

  void AudioEngine::SetGlobalParameterByName(const std::string& parameter, float value)
  {
    FMODWrapper::ErrorChecker(fmod->pStudioSystem->setParameterByName(parameter.c_str(), value));
  }

  float AudioEngine::dBToVolume(float dB)
  {
    return powf(10.0f, 0.05f * dB);
  }

  float AudioEngine::VolumeTodB(float volume)
  {
    return 20.0f * log10f(volume);
  }
}
