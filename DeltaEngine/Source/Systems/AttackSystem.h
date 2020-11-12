#pragma once
#include "ECS/ECSModule.h"
#include "Components/Attack.h"
#include "Components/Collider.h"
#include "Components/Health.h"
#include "Components/Lifespan.h"

namespace DeltaEngine
{
	DEFINE_SYSTEM(AttackSystem, Attack,Collider,Health,Lifespan,Transform,EntityType)
		void Update() override;
		void LateUpdate() override;
		void RangedAttack(EntityID&);
	END_DEFINE_SYSTEM(AttackSystem)
}
