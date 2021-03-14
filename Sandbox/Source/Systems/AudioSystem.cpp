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
  em.ForEach( [](EntityID& id, AudioSource& a)
  {
    if ( !a.isStart && !a.clip.empty())
    {
      if ( a.isEvent )
        AudioEngine::AudioSourcePlay2DEvent( a );
      else
        AudioEngine::AudioSourcePlay( a );

      a.isStart = true;
    } 
  } );
}

}