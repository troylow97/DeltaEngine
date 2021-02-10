/**********************************************************************************
* \file   Components.h
* \brief  The file contains BLAHBLAHBLAH
* \author Chin, Clara,   X% Code Contribution
* \author Low, Troy,     X% Code Contribution
* \author Ong, Graeme,   X% Code Contribution
* \author Tan, Tong Wee, X% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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
#include "ParticleEmitter.h"
#include "Player.h"
#include "UI.h"
#include "VideoPlayer.h"
#include "Render/Camera.h"
#include "Core/Typelist/Typelist.h"

namespace DeltaEngine
{
	using CoreBase = Typelist<EntityID, Parent, EntityName, EntityType>;
	using PhysicsBase = CoreBase::Append<Transform, RigidBody, Collider>;
	using RenderBase = PhysicsBase::Append<Image, Renderer2D, RendererOverlay, Text, Animator, State, Camera, VideoPlayer, ParticleEmitter>;
	using GameBase = RenderBase::Append< AI, Health, Attack, Lifespan, Player, Input, UI, GUI>;
	using ComponentList = GameBase;
}