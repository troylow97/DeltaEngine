#pragma once
#include "ECS/ECSModule.h"
#include "Components/Attack.h"
#include "Components/Collider.h"
#include "Components/Health.h"
#include "Components/Lifespan.h"
#include <vector>

namespace DeltaEngine
{
DEFINE_SYSTEM(AttackSystem, Attack, Collider, Health, Lifespan, Transform, EntityType, Image)
  //void Initialize() override;
  void Update() override;
  void LateUpdate() override;
  void RangedAttack(EntityID&);
  void MeleeAttack(EntityID&);
  void Dash();
  void AttackCombo();
private:
  std::vector<EntityID> MeleeAttackingEntities;
  std::vector<EntityID> RangedAttackingEntities;
  //PlayerAttackCombo _pac;
END_DEFINE_SYSTEM(AttackSystem)
}
