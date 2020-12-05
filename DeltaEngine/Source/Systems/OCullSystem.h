#pragma once
#include "DeltaEngine.h"

namespace DeltaEngine
{
  DEFINE_SYSTEM(OCullSystem, Transform)
    void Update() override;
    void LateUpdate() override;
  END_DEFINE_SYSTEM(OCullSystem)
}
