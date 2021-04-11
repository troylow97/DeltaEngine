/**********************************************************************************
* \file   HealthPickupSystem.h
* \brief  The file contains the system for healthpickups
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "ECS/ECSModule.h"
#include "Components/Components.h"
#include "../Source/Core/Math/Vector.h"
#include "../Source/Core/Utils/Random.h"

namespace DeltaEngine
{
  DEFINE_SYSTEM(HealthPickupSystem,Collider, Lifespan, Transform, EntityType)
  void Initialize() override;
  void Update() override;
  void LateUpdate() override;
  static void SpawnHealthOrbOnDeath(const Vector2 position);
private:
  static float PickupChance;
  static EntityID SpawnHealthOrb(const Vector2 position);
  END_DEFINE_SYSTEM(HealthPickupSystem)
}
