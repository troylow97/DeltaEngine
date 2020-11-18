#pragma once

#include "DeltaEngine.h"

namespace DeltaEngine
{
  DEFINE_SYSTEM(InputSystem, Input)
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;

    void SelectEntity();
    void DragEntity();
    void DragCamera();

    void Shutdown() override;
  END_DEFINE_SYSTEM(InputSystem)
}
