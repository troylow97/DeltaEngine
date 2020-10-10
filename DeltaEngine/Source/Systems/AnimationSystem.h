#pragma once

#include "DeltaEngine.h"

namespace DeltaEngine
{
DEFINE_SYSTEM( AnimationSystem, Animator, SpriteRenderer )
void Update() override;
void LateUpdate() override;
END_DEFINE_SYSTEM( AnimationSystem )
}
