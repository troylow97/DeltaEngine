#pragma once

#include "DeltaEngine.h"

namespace DeltaEngine
{
    DEFINE_SYSTEM(PhysicsDrawSystem, Transform, Collider)
    void update() override;
    void late_update() override;
    END_DEFINE_SYSTEM(RenderSystem)
}
