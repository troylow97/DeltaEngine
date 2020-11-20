#pragma once
#include "DeltaEngine.h"

namespace DeltaEngine
{
  DEFINE_SYSTEM(TextRenderSystem, Transform)
    void Update() override;
    void LateUpdate() override;
  END_DEFINE_SYSTEM(TextRenderSystem)
}
