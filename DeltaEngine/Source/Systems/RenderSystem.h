#pragma once

#include "DeltaEngine.h"

namespace DeltaEngine
{
    DEFINE_SYSTEM(RenderSystem, Transform, Renderer)
    void Update() override;
    void LateUpdate() override;
    END_DEFINE_SYSTEM(RenderSystem)
}
