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
    em.ForEach( [&]( EntityName &name, GUI &g, Image &i )
    {
      if ( name.name == "Dodge Cooldown" )
      {
        if ( player_attack->CurrentDodgeCooldown <= player_attack->DodgeCooldown )
          i.m_FillAmount = Math::Clamp01( player_attack->CurrentDodgeCooldown / player_attack->DodgeCooldown );
        else
          i.m_FillAmount = 1.0f;
      }

      if ( name.name == "Health" )
      {
        i.m_FillAmount = Math::Clamp01( static_cast<float>( player_health->CurrentHealth ) / static_cast<float>( player_health->MaxHealth ) );
      }
    } );
  }
}

void GUISystem::LateUpdate()
{

}


}