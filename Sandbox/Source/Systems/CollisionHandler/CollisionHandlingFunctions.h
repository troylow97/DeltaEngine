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
				
	};

}
