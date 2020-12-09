/**********************************************************************************
* \file   UISystem.cpp
* \brief  The file contains the system for updating and displaying UI in the game
* \author Chin, Clara,     70% Code Contribution
* \author Low, Troy,       30% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "UISystem.h"

#include <rttr/registration.h>
#include <rttr/detail/registration/registration_impl.h>

#include "UnitManager.h"
#include "Components/Components.h"
#include "EnemySpawner/EnemySpawner.h"
#include "ImGui/Panels/ViewportPanel.h"
#include "Physics/Collision.h"
#include "Input/InputManager.h"
#include "Input/Keys.h"
#include "Systems/OCullSystem.h"

namespace DeltaEngine
{
  const unsigned pause_screen = 0;
  const unsigned main_screen = 1;
  const unsigned interface = 2;
  const unsigned start_game_selected = 3;
  const unsigned control_selected = 4;
  const unsigned option_selected = 5;
  const unsigned credits_selected = 6;
  const unsigned restart_selected = 7;
  const unsigned quit_game_selected = 8;
  const unsigned control_screen = 9;
  const unsigned option_screen = 10;
  const unsigned credits_screen = 11;
  const unsigned gameover_screen = 12;
  const unsigned upgrade_page = 13;
  const unsigned level1_screen = 14; // Icon_HeavyAttack_2 && Icon_RangedAttack_2
  const unsigned upgraded_health_page = 15;
  const unsigned upgraded_attack_page = 16;
  const unsigned upgraded_health_only_page = 17;
  const unsigned upgraded_attack_only_page = 18;
  const unsigned using_dash = 19; // p.IsDashing
  const unsigned dash_not_ready = 20; // p.AllowDashing = false;
  const unsigned using_ranged = 21; // a.RangeAttack
  const unsigned ranged_not_ready = 22; // AttackCooldown > 0
  const unsigned quit_confirmation = 23;
  const unsigned quit_yes = 24;
  const unsigned quit_no = 25; // credits default - 4

  void UISystem::Initialize()
  {
    OCullSystem::Enable(true);
    m_screen.push_back(main_screen);
    is_main_menu = true;
    //m_screen.push_back(level1_screen);
    em.ForEach([&](UI& ui, Transform& t, Image& i, Renderer2D& r)
    {
      if (ui.ui_type == UIType::Slider)
        VolumeSliderInitialLocation = t.position;
    });
    isDraggingOnSlider = false;
  }

  void UISystem::AttackVisualFeedback()
  {
    if (env.pECS->GetWorld().GetEntityManager().IsEntityValid(UnitManager::GetPlayerID()))
      if (env.pECS->GetWorld().GetEntityManager().HasComponent<Player>(UnitManager::GetPlayerID()))
      {
        auto& p = env.pECS->GetWorld().GetEntityManager().GetComponent<Player>(UnitManager::GetPlayerID());
        auto& a = env.pECS->GetWorld().GetEntityManager().GetComponent<Attack>(UnitManager::GetPlayerID());

        if (p.IsDashing)
          m_screen.push_back(using_dash);
        else if (!p.IsDashing && !p.AllowDashing)
        {
          m_screen.clear();
          m_screen.push_back(level1_screen);
          m_screen.push_back(dash_not_ready);
        }
        if (p.AllowDashing)
        {
          m_screen.clear();
          m_screen.push_back(level1_screen);
        }

        if (a.RangeAttack)
          m_screen.push_back(using_ranged);
        else if (!a.RangeAttack && a.RangeCooldown < 0.0f)
        {
          m_screen.clear();
          m_screen.push_back(level1_screen);
          m_screen.push_back(ranged_not_ready);
        }
        //if (a.AttackCooldown > 0.0f)
        //{
        //    m_screen.clear();
        //    m_screen.push_back(level1_screen);
        //    m_screen.push_back(ranged_not_ready);
        //}_screen.push_back(level1_screen);}
      }
  }

  void UISystem::UpdateHealthBar()
  {
    env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& id, UI& u, Image& im, Transform& t, Renderer2D& r)
    {
      if (env.pECS->GetWorld().GetEntityManager().IsEntityValid(UnitManager::GetPlayerID()))
        if (env.pECS->GetWorld().GetEntityManager().HasComponent<Player>(UnitManager::GetPlayerID()))
        {
          auto& hp = em.GetComponent<Health>(UnitManager::GetPlayerID());
          auto& player_pos = em.GetComponent<Transform>(UnitManager::GetPlayerID());
          Vector2 translation = {-2.6f, 1.65f};

          if (u.ui_type == UIType::Healthbar)
          {
            const float current_hp = static_cast<float>(hp.CurrentHealth);
            const float max_hp = static_cast<float>(hp.MaxHealth);
            const float percentage = (current_hp / max_hp);
            im.m_FillAmount = percentage;
            r.m_Wireframe = false;
            t.position.x = player_pos.position.x + translation.x;
            t.position.y = translation.y;
          }
          else if (u.ui_type == UIType::Healthbar_base)
          {
            r.m_Wireframe = false;
            t.position.x = player_pos.position.x + translation.x;
            t.position.y = translation.y;
          }
        }
    });
  }

  void UISystem::Update()
  {
  }

  void UISystem::LateUpdate()
  {
    if (credits_rolling)
    {
      m_screen.clear();
      credits_rolling = false;
      return;
    }

    if (InputManager::Instance().IsKeyTriggered(DEVK_ESCAPE))
    {
      bool option_menu_bool{false};
      bool pause_screen_bool{false};

      for (auto& screen : m_screen)
      {
        if (screen == option_screen)
        {
          m_screen.clear();
          if (is_main_menu)
            m_screen.push_back(main_screen);
          else
          {
            m_screen.push_back(pause_screen);
            pause_screen_bool = true;
          }
          break;
        }
        if (screen == control_screen)
        {
          m_screen.clear();
          if (is_main_menu)
            m_screen.push_back(main_screen);
          else
          {
            m_screen.push_back(pause_screen);
            pause_screen_bool = true;
          }
          break;
        }
        if (screen == credits_screen)
        {
          m_screen.clear();
          if (is_main_menu)
            m_screen.push_back(main_screen);
          break;
        }
        if (screen == level1_screen)
        {
          m_screen.push_back(pause_screen);
          pause_screen_bool = true;
        }
        if (screen == pause_screen)
        {
          pause_screen_bool = false;
          break;
        }
      }
      if (!is_main_menu)
      {
        if (pause_screen_bool)
        {
          auto& p = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>({0});
          PlayerCurrentPosition = p.position;
          PlayerCurrentPosition.y = 0.0f;
          Vector3 difference;
          difference = PlayerCurrentPosition - PlayerFirstPosition;
          PlayerFirstPosition = PlayerCurrentPosition;

          em.ForEach([&](UI& ui, EntityID& id)
          {
            if (ui.screen == 0 || ui.screen == 4 || ui.screen == 5 || ui.screen == 7 || ui.screen == 8 || ui.screen == 9
              || ui.screen == 10 || ui.screen == 23 || ui.screen == 24 || ui.screen == 25)
              env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id).position += difference;
          });

          PauseGame();
        }
        else
        {
          m_screen.clear();
          UnpauseGame();
          m_screen.push_back(level1_screen);
        }
      }
    }

    if (!is_main_menu)
    {
      bool paused{false};

      auto& p = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>({0});
      UPlayerCurrentPosition = p.position;
      PlayerCurrentPosition.y = 0.0f;
      Vector3 difference;
      difference = UPlayerCurrentPosition - UPlayerFirstPosition;
      UPlayerFirstPosition = UPlayerCurrentPosition;

      em.ForEach([&](UI& ui, EntityID& id)
      {
        if (ui.screen == 13 || ui.screen == 17 || ui.screen == 18)
          env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id).position += difference;
      });

      //if ( InputManager::Instance().IsKeyTriggered( DEVK_U ) ) //Upgrade Page
      //{
      //  bool upgrade_screen_exists = false;
      //  auto &p = env.pECS->GetWorld().GetEntityManager().GetComponent<Player>( UnitManager::GetPlayerID() );
      //  for ( auto &screen : m_screen )
      //  {
      //    if ( screen == upgrade_page )
      //      upgrade_screen_exists = true;
      //  }
      //
      //  if ( upgrade_screen_exists )
      //  {
      //    m_screen.clear();
      //    m_screen.push_back( level1_screen );
      //  }
      //  else
      //  {
      //    m_screen.push_back( upgrade_page );
      //
      //    if ( p.UpgradedAtk )
      //      m_screen.push_back( upgraded_attack_only_page );
      //    if ( p.UpgradedHP )
      //      m_screen.push_back( upgraded_health_only_page );
      //  }
      //}
      for (auto& screen : m_screen)
      {
        if (screen == pause_screen || screen == control_screen || screen == option_screen || screen == gameover_screen
          || screen == upgrade_page || screen == quit_confirmation)
        {
          paused = true;
          break;
        }
      }
      if (!paused)
      {
        UpdateHealthBar();
        AttackVisualFeedback();
      }

      for (auto& screen : m_screen) //Upgrade page to follow player
      {
        if (screen == level1_screen)
        {
          if (env.pECS->GetWorld().GetEntityManager().IsEntityValid(UnitManager::GetPlayerID()))
            if (env.pECS->GetWorld().GetEntityManager().HasComponent<Player>(UnitManager::GetPlayerID()))
            {
              Vector3 player_pos = em.GetComponent<Transform>({0}).position;

              em.ForEach([&](UI& ui, EntityID& id, EntityName& en)
              {
                if (en.name == "DashReady" || en.name == "DashNotReady" || en.name == "UsingDash")
                  env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id).position = {
                    player_pos.x - 2.6f, -1.45f/*player_pos.y -0.3868f*/, 0.0f
                  };
                else if (en.name == "RangedReady" || en.name == "RangedNotReady" || en.name == "UsingRanged")
                  env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id).position = {
                    player_pos.x - 2.35f, -1.6f/*player_pos.y - 0.5368f*/, 0.0f
                  };
              });
            }
        }
      }
    }

    auto& t = em.GetComponent<Transform>({0});
    float cameraWidth = Camera::allCameras[0]->Max(t).x - Camera::allCameras[0]->Min(t).x;
    float cameraHeight = Camera::allCameras[0]->Max(t).y - Camera::allCameras[0]->Min(t).y;

#ifdef DE_EDITOR
  float cursorViewPortDistanceX = InputManager::Instance().CurrentPosition().point_x - GamePanel::render_pos.x;
  float cursorViewPortDistanceY = InputManager::Instance().CurrentPosition().point_y - GamePanel::render_pos.y;
  auto p_x = ( ( cursorViewPortDistanceX / GamePanel::render_size.x ) * cameraWidth ) + Camera::allCameras[0]->Min( t ).x;
  auto p_y = Camera::allCameras[0]->Max( t ).y - ( ( cursorViewPortDistanceY / GamePanel::render_size.y ) * cameraHeight );
#else

    float cursorViewPortDistanceX = InputManager::Instance().CurrentPosition().point_x - GetEnv().pWin->ClientTopLeft().
      point_x;
    float cursorViewPortDistanceY = InputManager::Instance().CurrentPosition().point_y - GetEnv().pWin->ClientTopLeft().
      point_y;
    auto p_x = ((cursorViewPortDistanceX / GetEnv().pWin->ClientRect().point_x) * cameraWidth) + Camera::allCameras[0]->
      Min(t).x;
    auto p_y = Camera::allCameras[0]->Max(t).y - ((cursorViewPortDistanceY / GetEnv().pWin->ClientRect().point_y) *
      cameraHeight);
#endif


    if (!m_screen.empty())
      em.ForEach([&](UI& ui, Transform& t, Image& i, Renderer2D& r)
      {
        r.m_Active = false;
        for (auto screen : m_screen)
          if (screen == ui.screen)
          {
            r.m_Active = true;
            const bool rect_mouse = CollisionIntersection_RectMouse(t.position, i.GetWorldSize(), {p_x, p_y});

            if (screen == m_screen.back())
            {
              if (ui.ui_type == UIType::Button && rect_mouse)
              {
                // Animation update
                if (InputManager::Instance().IsKeyReleased(DEVK_LBUTTON))
                {
                  if (!ui.functor_key.empty())
                  {
                    rttr::type::get<UISystem>().get_method(ui.functor_key.c_str()).invoke({*this});
                    if (ui.overlay && ui.target_screen != -1)
                      m_screen.push_back(ui.target_screen);
                  }
                  if (ui.overlay && ui.target_screen != -1)
                    m_screen.push_back(ui.target_screen);
                  else if (ui.target_screen != -1)
                  {
                    m_screen.clear();
                    m_screen.push_back(ui.target_screen);
                  }
                }
              }
              else if (ui.ui_type == UIType::Interface && rect_mouse)
              {
                // Animation update
                if (ui.overlay && ui.target_screen != -1)
                  m_screen.push_back(ui.target_screen);
                else if (ui.target_screen != -1)
                {
                  m_screen.clear();
                  m_screen.push_back(ui.target_screen);
                }
              }
              else if (ui.ui_type == UIType::Slider && rect_mouse)
              {
                if (InputManager::Instance().IsKeyPressed(DEVK_LBUTTON))
                {
                  if (p_x <= VolumeSliderInitialLocation.x && p_x >= (VolumeSliderInitialLocation.x - 2.7f))
                    t.position.x = p_x;
                }
              }
              if ((ui.ui_type == UIType::Button || ui.ui_type == UIType::Interface) && !rect_mouse)
              {
                if (UI_first_time)
                {
                  // Animation update
                  if (ui.overlay && ui.previous_screen != -1)
                    m_screen.push_back(ui.previous_screen);
                  else if (ui.previous_screen != -1)
                  {
                    m_screen.clear();
                    m_screen.push_back(ui.previous_screen);
                  }
                  UI_first_time = false;
                }
              }
            }
            if (screen == 13)
            {
              auto& p = env.pECS->GetWorld().GetEntityManager().GetComponent<Player>(UnitManager::GetPlayerID());
              if (p.UpgradedAtk && upgraded_Attack_not_pushed)
              {
                m_screen.push_back(upgraded_attack_only_page);
                upgraded_Attack_not_pushed = false;
              }
              if (p.UpgradedHP && upgraded_HP_not_pushed)
              {
                m_screen.push_back(upgraded_health_only_page);
                upgraded_HP_not_pushed = false;
              }
            }
          }
      });
    UI_first_time = true;

    if (m_start)
      Start();
    if (m_restart)
      Restart();
    if (m_backmenu)
      BackToMenu();
  }

  void UISystem::Return()
  {
    m_screen.pop_back();
  }

  void UISystem::UpgradeDamageButton()
  {
    auto& player = em.GetComponent<Player>(UnitManager::GetPlayerID());
    player.UpgradeAtk = true;
  }

  void UISystem::UpgradeHPButton()
  {
    auto& player = em.GetComponent<Player>(UnitManager::GetPlayerID());
    player.UpgradeHP = true;
  }

  void UISystem::Start()
  {
    env.pECS->GetWorld().GetEntityManager().Clear();
    env.pECS->GetWorld().Load("World/MainLevelV2.json");
    env.pClock->TimeScale(1.0f);
    m_screen.clear();
    is_main_menu = false;
    PlayerFirstPosition = {0.0f, 1.1f, 0.0f}; // p.position;
    UPlayerFirstPosition = {0.0f, 1.1f, 0.0f}; // p.position;
    m_screen.push_back(level1_screen);
    m_start = false;
  }

  void UISystem::StartGame()
  {
    m_start = true;
  }

  void UISystem::Restart()
  {
    env.pECS->GetWorld().FindOrCreateSystem<EnemySpawner>().Shutdown();
    env.pECS->GetWorld().GetEntityManager().Clear();
    env.pECS->GetWorld().Load("World/MainLevelV2.json");
    env.pClock->TimeScale(1.0f);
    env.pECS->GetWorld().FindOrCreateSystem<EnemySpawner>().Initialize();
    m_screen.clear();
    is_main_menu = false;
    PlayerFirstPosition = {0.0f, 1.1f, 0.0f}; // p.position;
    UPlayerFirstPosition = {0.0f, 1.1f, 0.0f}; // p.position;
    m_screen.push_back(level1_screen);
    m_restart = false;
  }

  void UISystem::RestartGame()
  {
    m_restart = true;
  }

  void UISystem::QuitGame()
  {
    env.pECS->GetWorld().FindOrCreateSystem<EnemySpawner>().Shutdown();
    env.pWin->Running(false);
  }

  void UISystem::BackToMenu()
  {
    JsonFile file;
    env.pECS->GetWorld().GetEntityManager().Clear();
    env.pECS->GetWorld().Load("World/MainMenu.json");
    env.pClock->TimeScale(1.0f);
    is_main_menu = true;
    m_screen.clear();
    m_screen.push_back(main_screen);
    m_backmenu = false;
  }

  void UISystem::BackToMainMenu()
  {
    m_backmenu = true;
  }

  void UISystem::PauseGame()
  {
    env.pClock->TimeScale(0.0f);
  }

  void UISystem::UnpauseGame()
  {
    env.pClock->TimeScale(1.0f);
  }

  RTTR_REGISTRATION
  {
    rttr::registration::class_<UISystem>("UISystem")
      .method("Return", &UISystem::Return);

    rttr::registration::class_<UISystem>("UpgradeDamageButton")
      .method("UpgradeDamageButton", &UISystem::UpgradeDamageButton);

    rttr::registration::class_<UISystem>("UpgradeHPButton")
      .method("UpgradeHPButton", &UISystem::UpgradeHPButton);

    rttr::registration::class_<UISystem>("PauseGame")
      .method("PauseGame", &UISystem::PauseGame);

    rttr::registration::class_<UISystem>("UnpauseGame")
      .method("UnpauseGame", &UISystem::UnpauseGame);

    rttr::registration::class_<UISystem>("StartGame")
      .method("StartGame", &UISystem::StartGame);

    rttr::registration::class_<UISystem>("RestartGame")
      .method("RestartGame", &UISystem::RestartGame);

    rttr::registration::class_<UISystem>("QuitGame")
      .method("QuitGame", &UISystem::QuitGame);

    rttr::registration::class_<UISystem>("BackToMainMenu")
      .method("BackToMainMenu", &UISystem::BackToMainMenu);

    rttr::registration::class_<UISystem>("PauseGame")
      .method("PauseGame", &UISystem::PauseGame);
  }
}
