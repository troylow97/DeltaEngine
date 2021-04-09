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
#include "GUISystem.h"


#include <rttr/registration.h>

#include "Audio/AudioEngine.h"
#include "EnemySpawner/EnemySpawner.h"
#include "Systems/UISystem.h"
#include "RespawnSystem.h"
#include "Input/InputManager.h"
#include "Input/Keys.h"

namespace DeltaEngine
{

bool attacked { false };
bool healed { false };
bool changing { false };
float target { 0 };
float current { 0 };
float diff { 0.0f };
float timer { 0.5f };

void GUISystem::Update()
{
  Attack *player_attack { nullptr };
  Health *player_health { nullptr };

  em.ForEach( [&]( Player &p, Attack &a, Health &h )
  {
    player_attack = &a;
    player_health = &h;
  } );

  if ( player_attack && player_health )
  {

    if ( !changing )
      current = player_health->CurrentHealth;
    target = player_health->CurrentHealth;


    em.ForEach( [&]( EntityName &name, GUI &g, Image &i, State &s )
    {
      if ( name.name == "Dodge Cooldown" )
        s.SetFloat( "Cooldown", player_attack->CurrentDodgeCooldown );

      if ( name.name == "Health" )
      {
        s.SetBool( "Hit", attacked && changing );
        s.SetBool( "Heal", attacked && changing );

        auto p = Math::Clamp01( current / static_cast<float>( player_health->MaxHealth ) );
        s.SetFloat( "Health", p );
        i.m_FillAmount = p;
      }
    } );

    for ( size_t i = 0; i < GetEnv().pClock->Timesteps(); ++i )
      current += diff * GetEnv().pClock->FixedDeltaTime();



    if ( changing )
    {
      if ( diff > 0.0f && current > target )
        changing = healed = attacked = false;
      else if ( diff < 0.0f && current < target )
        changing = healed = attacked = false;
    }
  }
}

void GUISystem::LateUpdate()
{

}

void GUISystem::Attacked( bool b )
{
  attacked = true;
  changing = true;
  diff = target - current;
  diff /= timer;
}

void GUISystem::Heal( bool b )
{
  healed = true;
  changing = true;
  diff = target - current;
  diff /= timer;
}



}