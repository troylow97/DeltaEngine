#pragma once
#include "DeltaEngine.h"
namespace DeltaEngine
{
  DEFINE_SYSTEM(UISystem, Transform,Image,Renderer2D)
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;

    void AttackVisualFeedback();
    void UpdateHealthBar();
    void Start();
    void Restart();

    void Return();
    void UpgradeDamageButton();
    void UpgradeHPButton();
    void StartGame();
    void RestartGame();
    void QuitGame();
    void PauseGame();
    void UnpauseGame();
    void BackToMenu();
    void BackToMainMenu();
private:
  bool isDraggingOnSlider;
  bool UI_first_time = true;
  bool upgraded_HP_not_pushed = true;
  bool upgraded_Attack_not_pushed = true;
  bool back_to_main_menu = false;
  bool is_main_menu = false;
  bool m_start { false };
  bool m_restart { false };
  bool m_backmenu { false };

  Vector2 VolumeSliderInitialLocation;
  Vector3 PlayerFirstPosition;
  Vector3 PlayerCurrentPosition;
  Vector3 UPlayerFirstPosition;
  Vector3 UPlayerCurrentPosition;
  std::vector<unsigned> m_screen;
  END_DEFINE_SYSTEM(GCameraSystem)
}
