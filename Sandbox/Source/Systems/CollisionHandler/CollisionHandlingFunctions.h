#pragma once
#include "ECS/Entities.h"
#include "Components/EntityType.h"

namespace DeltaEngine
{
  void TakeDamage(EntityID& id1, EntityID& id2);
  bool CheckEntityType(EntityID id1, EntityCategory typecheck1, EntityID id2, EntityCategory typecheck2);
  void ReduceHealth(EntityID& id, int health);
}
