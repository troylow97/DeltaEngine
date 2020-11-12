#pragma once
#include <string>

namespace DeltaEngine
{
    enum class entity_type : unsigned int
    {
        E_WALL,
        E_PLAYER,
        E_ENEMY,
        E_PLAYER_BULLET,
        E_PLAYER_PUNCH,
        E_LANCER_CHARGE
    };

	struct EntityType
	{
        entity_type type;
	};
}
