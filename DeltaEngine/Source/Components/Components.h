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
#include "Player.h"
#include "UI.h"
#include "Render/Camera.h"
#include "Core/Typelist/Typelist.h"

namespace DeltaEngine
{
  using CoreBase = Typelist<EntityID,Parent, EntityName, EntityType>;
  using PhysicsBase = CoreBase::Append<Transform, RigidBody, Collider>;
  using RenderBase = PhysicsBase::Append<Image, Renderer2D, Text, Animator, State, Camera>;
  using GameBase = RenderBase::Append< AI, Health, Attack, Lifespan, Player, Input, UI>;
  using ComponentList = GameBase;
}
