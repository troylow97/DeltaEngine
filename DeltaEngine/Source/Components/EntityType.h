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
    E_PLAYER_PUNCH,
    E_LANCER_CHARGE,
    E_ENEMY_BULLET
  };

  struct EntityType
  {
    EntityCategory type;
  };
}
