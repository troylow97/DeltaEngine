/**********************************************************************************
* \file   EnemyData.h
* \brief  The file contains the class for EnemyData, to be used for spawning enemies
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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
