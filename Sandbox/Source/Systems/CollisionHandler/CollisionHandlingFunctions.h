/**********************************************************************************
* \file   CollisionHandlingFunctions.h
* \brief  The file contains functions that would be run during Collision Handling
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "../EnemySpawner/EnemyData.h"
#include "ECS/Entities.h"
#include "Components/EntityType.h"

namespace DeltaEngine
{
  class CollisionHandlerFunctions
  {
    static EnemyData CollisionHandlerFiddlerData;
    static EnemyData CollisionHandlerLancerData;
    static EnemyData CollisionHandlerSerpentipedeData;

  public:
    static void Initialise();
    static void TakeDamage(EntityID& id1, EntityID& id2);
    static bool CheckEntityType(EntityID, EntityCategory, EntityID, EntityCategory);

    static void ReduceHealth(EntityID& id, int health);
    static bool CheckBlock(EntityID& defender, EntityID& attacker);
  };
}
