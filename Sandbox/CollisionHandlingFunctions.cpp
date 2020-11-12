#include "CollisionHandlingFunctions.h"
#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"

namespace DeltaEngine
{
	void TakeDamage(EntityID& id1, EntityID& id2)
	{
		if (env.pECS->GetWorld().GetEntityManager().HasComponent<Health>(id1) && 
			env.pECS->GetWorld().GetEntityManager().HasComponent<EntityType>(id1) && 
			env.pECS->GetWorld().GetEntityManager().HasComponent<Health>(id2) &&
			env.pECS->GetWorld().GetEntityManager().HasComponent<EntityType>(id2)
			)
		{
			if(env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(id1).type != "wall" &&
				env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(id2).type != "wall")
			{
				auto& ref1 = env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(id1);
				ref1.CurrentHealth--;
				auto& ref2 = env.pECS->GetWorld().GetEntityManager().GetComponent<Health>(id2);
				ref2.CurrentHealth--;
			}

		}

	}
}