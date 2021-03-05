/**********************************************************************************
* \file   AttackSystem.h
* \brief  The file contains the system for gameplay attack
* \author Chin, Clara,   50% Code Contribution
* \author Low, Troy,     50% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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
    // void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void RangedAttack(EntityID&);
    void MeleeAttack(EntityID&);
    void SMGAttack(EntityID&);
    void Dash();
    void AttackCombo();
  private:
    EntityID CreateProjectile(EntityID id, Vector2 scale, bool gravity, float Lifetime, EntityCategory type);
    EntityID CreateSMGBullet(EntityID id, Vector2 scale, bool gravity, float Lifetime, EntityCategory type);
    std::vector<EntityID> MeleeAttackingEntities;
    std::vector<EntityID> RangedAttackingEntities;
    Vector2 CalculateAttackDirection(EntityID& enemy);
    // PlayerAttackCombo _pac;
  END_DEFINE_SYSTEM(AttackSystem)
}
