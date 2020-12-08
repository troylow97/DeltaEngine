#pragma once
#include "Core/Math/Vector.h"

namespace DeltaEngine
{
    struct EnemyData
    {
        Vector2 TransformScale;
        Vector2 ColliderScale;
        Vector2 ColliderOffset;
        int Health;
        float Movespeed;
        float Mass;
        float Damage;
        float Attackspeed;
    };

}
