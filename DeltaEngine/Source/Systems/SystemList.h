#pragma once

#include "Core/Typelist/Typelist.h"

#include "AnimationSystem.h"
#include "InputSystem.h"
#include "PhysicsDrawSystem.h"
#include "RenderSystem.h"

namespace DeltaEngine
{
using SystemList = Typelist<AnimationSystem, InputSystem, PhysicsDrawSystem, RenderSystem >;
}
