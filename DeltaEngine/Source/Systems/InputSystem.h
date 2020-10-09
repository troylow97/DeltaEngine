#pragma once

#include "DeltaEngine.h"

namespace DeltaEngine
{
    DEFINE_SYSTEM(InputSystem, Input)
    void update() override;
    void late_update() override;
    END_DEFINE_SYSTEM(InputSystem)
}