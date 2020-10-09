#pragma once

#include "DeltaEngine.h"

namespace DeltaEngine
{
    DEFINE_SYSTEM(InputSystem, InputManager)
    void update() override;
    END_DEFINE_SYSTEM(InputSystem)
}