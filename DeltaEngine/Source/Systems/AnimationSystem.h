#pragma once

#include "DeltaEngine.h"

namespace DeltaEngine
{
DEFINE_SYSTEM( AnimationSystem, Animator, State, Image )
void Update() override;
void LateUpdate() override;
END_DEFINE_SYSTEM( AnimationSystem )
}
