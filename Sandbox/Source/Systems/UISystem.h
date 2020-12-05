#pragma once
#include "DeltaEngine.h"
namespace DeltaEngine
{
  DEFINE_SYSTEM(UISystem, Transform)
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;


    void Return();
    void UpgradeDamageButton();
private:

  std::vector<unsigned> m_screen;
  END_DEFINE_SYSTEM(GCameraSystem)
}
