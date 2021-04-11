/**********************************************************************************
* \file   AudioSystem.cpp
* \brief  The file contains the definition for AudioSystem
* \author Tan Tong Wee     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "AudioSystem.h"


#include <rttr/registration.h>

#include "Audio/AudioEngine.h"
#include "Components/AudioSource.h"

namespace DeltaEngine
{

void AudioSystem::Update()
{

}

void AudioSystem::LateUpdate()
{
  em.ForEach( [&]( EntityID &id, AudioSource &a )
  {
    // Update all 3D Position
    if ( a.isStart && a.is3D && a.isPlayed && a.isPlaying )
    {
      auto &t = em.GetComponent<Transform>( id );
      if ( a.isEvent )
        AudioEngine::SetEvent3DAttribute( a.id, { t.position, {},{0,0,1},{0,1,0} } );
      else
        AudioEngine::SetChannel3DPosition( a.id, t.position );
    }

    // Update all playing status
    if ( a.isStart )
    {
      if ( a.isEvent )
        a.isPlaying = AudioEngine::IsEventPlaying( a.id );
      else
        a.isPlaying = AudioEngine::IsChannelPlaying( a.id );
    }
    else
      a.isPlaying = false;

    if ( a.isPlaying )
      a.isPlayed = true;
    else if ( !a.isPlaying && a.isPlayed )
    {
      if ( !a.isLoop )
        a.clip.clear();
      a.isPlayed = a.isStart = false;
    }

    if ( !a.isPlaying && !a.isPlayed && !a.isStart && !a.clip.empty() )
    {
      if ( a.isEvent )
      {
        if ( a.is3D )
        {
          auto &t = em.GetComponent<Transform>( id );
          AudioEngine::AudioSourcePlay3DEvent( a, { t.position, {},{0,0,1},{0,1,0} } );
        }
        else
          AudioEngine::AudioSourcePlay2DEvent( a );
      }
      else
      {
        if ( a.is3D )
        {
          auto &t = em.GetComponent<Transform>( id );
          AudioEngine::AudioSourcePlay3D( a, t.position );

        }
        else
          AudioEngine::AudioSourcePlay( a );
      }
    }

  } );
}

}