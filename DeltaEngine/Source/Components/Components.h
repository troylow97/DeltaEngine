#pragma once

#include "ECS/Entities.h"
#include "Components/Transform.h"
#include "Components/Character.h"
#include "Components/Collider.h"
#include "Components/RigidBody.h"
#include "Components/Renderer.h"
#include "Components/Text.h"
#include "Components/Image.h"
#include "Render/Animator.h"
#include "Core/Typelist/Typelist.h"

namespace DeltaEngine
{
using ComponentList = Typelist< EntityID, Input,  Collider,  Transform,  RigidBody, Animator, Image, Text, Renderer2D>;
}