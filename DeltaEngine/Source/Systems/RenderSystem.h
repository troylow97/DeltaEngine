#pragma once

#include "DeltaEngine.h"

namespace DeltaEngine
{
    DEFINE_SYSTEM(RenderSystem, Transform, Renderer)
    void update() override;
    void late_update() override;
    END_DEFINE_SYSTEM(RenderSystem)
}
