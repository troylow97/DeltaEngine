#pragma once

#include "DeltaEngine.h"

namespace DeltaEngine
{
    DEFINE_SYSTEM(InputSystem, Input)
    void Update() override;
    void LateUpdate() override;
    END_DEFINE_SYSTEM(InputSystem)
}