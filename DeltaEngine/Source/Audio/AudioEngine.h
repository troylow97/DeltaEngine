/**********************************************************************************
* \file   AudioEngine.h
* \brief  This file contains the definition of the Audio Engine that interacts with
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

#pragma once

#include <unordered_map>
#include "Core/Math/Vector.h"

using AUDIOENGINE_LOAD_BANK_FLAGS = unsigned;
#define AUDIOENGINE_LOAD_BANK_NORMAL             0x00000000
#define AUDIOENGINE_LOAD_BANK_NONBLOCKING        0x00000001
#define AUDIOENGINE_LOAD_BANK_DECOMPRESS_SAMPLES 0x00000002
#define AUDIOENGINE_LOAD_BANK_UNENCRYPTED        0x00000004

namespace DeltaEngine
{
  struct Audio3DAttributes
  {
    Vector3 pos;
    Vector3 vel;
    Vector3 forward;
    Vector3 up;
  };

  class AudioEngine
  {
    using EventID = unsigned;
    using ChannelID = unsigned;

  public:
    using ParametersMap = std::unordered_map<std::string, float>;

    static void Initialize();
    static void Shutdown();
    static void Update();


    static void AudioSourcePlay( struct AudioSource & );
    static void AudioSourcePlay2DEvent( struct AudioSource &, ParametersMap = ParametersMap() );
    static void AudioSourcePlay3DEvent( struct AudioSource &, Audio3DAttributes attributes, ParametersMap = ParametersMap() );
    static void StopAllAudio();

    // Core 
    static void LoadSound(const std::string& name, bool loop = false, bool stream = false, bool is3D = false); // shift
    static void UnloadSound(const std::string& name); // shift

    static ChannelID Play(const std::string& name, float volume = 1.0f, Vector3 pos = {0.0f, 0.0f, 0.0f}); // shift
    static bool IsChannelPlaying(ChannelID id);
    static void SetChannelPause(ChannelID id, bool pause = true);
    static void SetChannelPitch(ChannelID id, float pitch);
    static void SetChannelVolume(ChannelID id, float volume);
    static void SetChannelMode( ChannelID id, unsigned mode );
    static void SetChannelLoop( ChannelID id );
    static void SetChannelLoopCount( ChannelID id, int count );
    static void StopChannel(ChannelID id);
    static void StopChannels();

    static void SetChannel3DPosition(ChannelID id, Vector3 attributes);
    static unsigned GetChannelPlaybackPosition(ChannelID id);
    static unsigned GetSoundLength(const std::string& name);

    // Studio
    static bool IsLoadedBank(const std::string& name);
    static void LoadBank(const std::string& name, AUDIOENGINE_LOAD_BANK_FLAGS flags);
    static void UnloadBank(const std::string& name);
    static std::vector<std::string> EventList(const std::string& name);

    static EventID Play3DEvent(const std::string& name, Audio3DAttributes attributes, ParametersMap = ParametersMap());
    static EventID Play2DEvent(const std::string& name, ParametersMap = ParametersMap());
    static bool IsEventPlaying(EventID id);
    static void SetEventPause(EventID id, bool pause = true);
    static void SetEventPitch(EventID id, float pitch);
    static void SetEventVolume(EventID id, float volume);
    static void StopEvent(EventID id, bool fade = true);

    static void SetEvent3DAttribute(EventID id, Audio3DAttributes attributes);
    static float GetEventParameterByName(EventID id, const std::string& parameter);
    static void SetEventParameterByName(EventID id, const std::string& parameter, float value);

    static void SetBusMute(const std::string& name, bool mute = true);
    static void SetBusPause(const std::string& name, bool pause = true);
    static void SetBusVolume(const std::string& name, float volume);
    static void StopAllBusEvents(const std::string& name, bool fade = true);

    // Misc
    static void Set3DListenerAttributes(Audio3DAttributes attributes);
    static float GetGlobalParameterByName(const std::string& parameter);
    static void SetGlobalParameterByName(const std::string& parameter, float value);
    static float dBToVolume(float dB);
    static float VolumeTodB(float volume);
  };
}
