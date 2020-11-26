#pragma once

#include "ECS/Entities.h"
#include "Transform.h"
#include "Character.h"
#include "Collider.h"
#include "RigidBody.h"
#include "Renderer.h"
#include "Text.h"
#include "Image.h"
#include "Animator.h"
#include "State.h"
#include "EntityInfo.h"
#include "AI.h"
#include "EntityType.h"
#include "Health.h"
#include "Attack.h"
#include "Lifespan.h"
#include "Core/Typelist/Typelist.h"


namespace DeltaEngine
{
  using CoreBase = Typelist<EntityID,Parent, EntityName, EntityType>;
  using PhysicsBase = CoreBase::Append<Transform, RigidBody, Collider>;
  using RenderBase = PhysicsBase::Append<Image, Renderer2D, Text, Animator, State>;
  using GameBase = RenderBase::Append< Input, AI, Health, Attack, Lifespan>;
  using ComponentList = GameBase;
}
