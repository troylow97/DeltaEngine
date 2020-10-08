#pragma once

#include "DeltaEngine.h"

namespace DeltaEngine
{
    DEFINE_SYSTEM(AnimationSystem, Animator, SpriteRenderer)
    void update() override;
    void late_update() override;
    END_DEFINE_SYSTEM(AnimationSystem)
}
