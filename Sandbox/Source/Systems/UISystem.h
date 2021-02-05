/**********************************************************************************
* \file   UISystem.h
* \brief  The file contains the system for updating and displaying UI in the game
* \author Chin, Clara,     70% Code Contribution
* \author Low, Troy,       30% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "DeltaEngine.h"

namespace DeltaEngine
{
  DEFINE_SYSTEM(UISystem, Transform, Image, Renderer2D)
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
    inline static bool credits_rolling{false};
  private:
    bool isDraggingOnSlider{false};
    bool UI_first_time{true};
    bool upgraded_HP_not_pushed{true};
    bool upgraded_Attack_not_pushed{true};
    bool back_to_main_menu{false};
    bool is_main_menu{false};
    bool m_start{false};
    bool m_restart{false};
    bool m_backmenu{false};
    bool pause_screen_bool{false};


    Vector2 VolumeSliderInitialLocation;
    Vector3 PlayerFirstPosition;
    Vector3 PlayerCurrentPosition;
    Vector3 UPlayerFirstPosition;
    Vector3 UPlayerCurrentPosition;
    std::vector<unsigned> m_screen;
  END_DEFINE_SYSTEM(GCameraSystem)
}
