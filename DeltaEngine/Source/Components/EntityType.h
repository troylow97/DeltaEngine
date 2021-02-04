/**********************************************************************************
* \file   EntityType.h
* \brief  The file contains components related to EntityTypes
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

namespace DeltaEngine
{
  enum class EntityCategory : unsigned
  {
    E_NONE,
    E_WALL,
    E_PLAYER,
    E_ENEMY,
    E_PLAYER_BULLET,
    E_PLAYER_BULLET_DETECTION,
    E_PLAYER_PUNCH,
    E_PLAYER_DASH,
    E_PLAYER_SMG,
    E_ENEMY_BULLET,
    E_ENEMY_FIDDLER_PUNCH,
    E_ENEMY_LANCER_PUNCH
  };

  struct EntityType
  {
    EntityCategory type{EntityCategory::E_NONE};
  };
}
