#pragma once
#include "DeltaEngine.h"
namespace DeltaEngine
{
  DEFINE_SYSTEM(UISystem, Transform)
    void Update() override;
    void LateUpdate() override;


    void Return();

private:

  std::vector<unsigned> m_screen;
  END_DEFINE_SYSTEM(GCameraSystem)
}
