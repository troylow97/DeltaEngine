#pragma once

#include "ECS/ECSModule.h"
#include "Components/Transform.h"

namespace DeltaEngine
{
  DEFINE_SYSTEM(RenderSystem, Transform)
    void Update() override;
    void LateUpdate() override;
  END_DEFINE_SYSTEM(RenderSystem)
}
