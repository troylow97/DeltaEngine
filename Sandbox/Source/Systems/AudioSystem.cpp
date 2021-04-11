/**********************************************************************************
* \file   UISystem.cpp
* \brief  The file contains the system for updating and displaying UI in the game
* \author Chin, Clara,     70% Code Contribution
* \author Low, Troy,       30% Code Contribution
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

  em.ForEach( []( EntityID &id, Player &p, Transform &t )
  {
    AudioEngine::Set3DListenerAttributes( { t.position, {},{0,0,1},{0,1,0} });
  } );

  em.ForEach( [&]( EntityID &id, AudioSource &a )
  {
    if ( a.isEvent )
      a.isPlaying = AudioEngine::IsEventPlaying( a.id );
    else
      a.isPlaying = AudioEngine::IsChannelPlaying( a.id );

    if ( !a.isPlaying && a.isStart )
    {
      if (!a.isLoop )
        a.clip.clear();
      a.isStart = false;
    }

    if ( !a.isStart && !a.clip.empty() )
    {
      if ( a.isEvent )
        if ( a.is3D )
        {
          auto &t = em.GetComponent<Transform>( id );
          AudioEngine::AudioSourcePlay3DEvent( a, { t.position, {},{0,0,1},{0,1,0} } );
        }
        else
          AudioEngine::AudioSourcePlay2DEvent( a );
      else
        AudioEngine::AudioSourcePlay( a );

      a.isStart = true;
    }

    if ( a.isStart && a.is3D )
    {
      auto &t = em.GetComponent<Transform>( id );
      if ( a.isEvent )
        AudioEngine::SetEvent3DAttribute( a.id, { t.position, {},{0,0,1},{0,1,0} } );
      else
        AudioEngine::SetChannel3DPosition( a.id, t.position);
    }

  } );
}

}