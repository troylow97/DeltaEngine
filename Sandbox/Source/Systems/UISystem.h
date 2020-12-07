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
    void UnpauseGame();
    void BackToMainMenu();
private:
  bool isDraggingOnSlider;
  bool UI_first_time = true;
  Vector2 VolumeSliderInitialLocation;
  Vector3 PlayerFirstPosition;
  Vector3 PlayerCurrentPosition;
  std::vector<unsigned> m_screen;
  END_DEFINE_SYSTEM(GCameraSystem)
}
