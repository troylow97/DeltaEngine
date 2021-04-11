/**********************************************************************************
* \file   UnitManager.cpp
* \brief  The file contains the system for getting specific unit's ID
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "UnitManager.h"

#include "Core/GlobalStruct.h"
#include "Core/GameClock/EngineClock.h"
#include "Audio/AudioEngine.h"
namespace DeltaEngine
{
EntityID UnitManager::player;

bool updated { false };

void UnitManager::Update()
{
  env.pECS->GetWorld().GetEntityManager().ForEach( [&]( EntityID &id, Collider &c, Player &p, RigidBody &r, State &s, Animator &a, Health &hp, AudioSource &audio )
  {
    //if(hp.isDamagedTimer > 0.0f)
    //{
    //    return;
    //}
    //std::cout << "player clip is " << a.m_ClipKey << std::endl;
    const int velocity = static_cast<int>( r.Velocity.y );
    //std::cout << "velocity is: " << velocity << std::endl;

    s.SetFloat( "VelocityY", static_cast<float>( velocity ) );

    static float jump = 0;
    static bool fall = false;
    s.SetFloat( "VelocityY", static_cast<float>( velocity ) );
    if ( velocity > 0 )
    {
      //std::cout << "here1" << std::endl;
      s.SetBool( "Jump", true );
    }
    if ( s.GetBool( "Jump" ) )
    {
      if ( velocity < 0 )
      {
        //std::cout << "here2" << std::endl;
        fall = true;
        jump += static_cast<float>( GetEnv().pClock->FixedDeltaTime() );
        s.SetFloat( "Jump", jump );
      }

      if ( fall )
      {
        if ( c.isCollidingOnFloor )
        {
          //std::cout << "here3" << std::endl;
          s.SetBool( "Jump", false );
          s.SetBool( "VelocityY", true );
          p.IsJumping = false;
          fall = false;
          jump = 0;
        }
      }
    }
    else if ( s.GetBool( "VelocityY" ) ) // recovering
    {
      if ( a.m_ClipKey == "Clip/DAVE_LAND" && a.LoopsCompleted() )
      {
        s.SetBool( "VelocityY", false );
        s.SetBool( "IsIdle", true );
      }
    }

    if ( c.isCollidingOnFloor && a.m_ClipKey == "Clip/DAVE_FALL" )
    {
      audio.clip = "event:/Player/Player Land";
      AudioEngine::AudioSourcePlay2DEvent( audio );

      s.SetBool( "Jump", false );
      s.SetBool( "VelocityY", true );
      p.IsJumping = false;
      fall = false;
      jump = 0;

    }

    player = id;
  } );
  updated = true;
}


EntityID UnitManager::GetPlayerID()
{
  if ( !updated )
    UnitManager::Update();

  return player;
}

void UnitManager::Reset()
{
  updated = false;
}
}
