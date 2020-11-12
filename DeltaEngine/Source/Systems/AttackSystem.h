#pragma once
#include "ECS/ECSModule.h"
#include "Components/Attack.h"
#include "Components/Collider.h"
#include "Components/Health.h"
#include "Components/Lifespan.h"
#include <vector>
namespace DeltaEngine
{
	DEFINE_SYSTEM(AttackSystem, Attack,Collider,Health,Lifespan,Transform,EntityType,Image)
		void Update() override;
		void LateUpdate() override;
		void RangedAttack(EntityID&);
		void MeleeAttack(EntityID&);
private:
	std::vector<EntityID>MeleeAttackingEntities;
	std::vector<EntityID>RangedAttackingEntities;
	END_DEFINE_SYSTEM(AttackSystem)
}
