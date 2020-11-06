#pragma once

#include "ECS/Entities.h"
#include "Components/Transform.h"
#include "Components/Character.h"
#include "Components/Collider.h"
#include "Components/RigidBody.h"
#include "Components/Renderer.h"
#include "Components/Text.h"
#include "Components/Image.h"
#include "Components/Animator.h"
#include "Components/State.h"
#include "Core/Typelist/Typelist.h"
#include "AI.h"
namespace DeltaEngine
{
using ComponentList = Typelist< EntityID, Input,  Collider,  Transform,  RigidBody, Animator, State, Image, Text, Renderer2D, AI>;
}