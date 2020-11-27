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
    E_PLAYER_PUNCH_COMBO,
    E_PLAYER_DASH,
    E_ENEMY_BULLET,
    E_ENEMY_FIDDLER_PUNCH
  };

  struct EntityType
  {
    EntityCategory type {EntityCategory::E_NONE};
  };
}
