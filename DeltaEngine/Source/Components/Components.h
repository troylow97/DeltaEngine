#pragma once

#include "ECS/Entities.h"
#include "Components/Transform.h"
#include "Components/Character.h"
#include "Components/Collider.h"
#include "Components/RigidBody.h"
#include "Render/SpriteRenderer.h"
#include "Render/Animator.h"
#include "Render/Renderer.h"
#include "Render/TextRenderer.h"
#include "Render/ParticleSystem.h"
#include "Core/Typelist/Typelist.h"
#include "AI.h"
namespace DeltaEngine
{
  using ComponentList = Typelist<EntityID, Input, Collider, Transform, RigidBody, SpriteRenderer, Animator, TextRenderer
                                 , ParticleSystem,AI>;
}
