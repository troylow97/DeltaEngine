#pragma once
#include "DeltaEngine.h"
namespace DeltaEngine
{
  DEFINE_SYSTEM(UISystem, Transform,Image,Renderer2D)
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;


    void Return();
    void UpgradeDamageButton();
    void UpgradeHPButton();
    void StartGame();
    void QuitGame();
    void PauseGame();
    void BackToMainMenu();
private:
  bool isDraggingOnSlider;
  Vector2 VolumeSliderInitialLocation;
  std::vector<unsigned> m_screen;
  END_DEFINE_SYSTEM(GCameraSystem)
}
