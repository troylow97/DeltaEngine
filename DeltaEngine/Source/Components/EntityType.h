#pragma once
#include <string>

namespace DeltaEngine
{
    enum class entity_type : unsigned int
    {
        E_WALL = 0x01,
        E_PLAYER = 0x02,
        E_ENEMY = 0x04,
        E_BULLET = 0x08
    };

	struct EntityType
	{
        entity_type type;
	};
}
