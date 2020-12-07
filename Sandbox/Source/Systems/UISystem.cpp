#include "UISystem.h"

#include <rttr/registration.h>
#include <rttr/detail/registration/registration_impl.h>

#include "UnitManager.h"
#include "Components/Components.h"
#include "EnemySpawner/EnemySpawner.h"
#include "ImGui/Panels/GamePanel.h"
#include "ImGui/Panels/ViewportPanel.h"
#include "Physics/Collision.h"
#include "Input/InputManager.h"
#include "Input/Keys.h"

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
const unsigned level1_screen = 14;
const unsigned upgraded_health_page = 15;
const unsigned upgraded_attack_page = 16;
const unsigned upgraded_health_only_page = 17;
const unsigned upgraded_attack_only_page = 18;

void UISystem::Initialize()
{
  //m_screen.push_back(main_screen);
  m_screen.push_back(level1_screen);
  em.ForEach([&](UI& ui, Transform& t, Image& i, Renderer2D& r)
  {
    if (ui.ui_type == UIType::Slider)
      VolumeSliderInitialLocation = t.position;
  });
  isDraggingOnSlider = false;
}
	
void UISystem::Update()
{

}

void UISystem::LateUpdate()
{
  //if ( InputManager::Instance().IsKeyReleased( DEVK_ESCAPE ) )
  //{
  //  bool main { false };
  //  bool pause { false };
  //  for ( auto screen : m_screen )
  //  {
  //    if ( screen == main_screen )
  //      main = true;
  //    else if ( screen == pause_screen )
  //    {
  //      pause = true;
  //      m_screen.clear();
  //      break;
  //    }
  //  }
  //	
  //  if ( !main && !pause)
  //  {
  //    m_screen.clear();
  //    m_screen.push_back( pause_screen );
  //  }
  //
  //}

  if (InputManager::Instance().IsKeyTriggered(DEVK_ESCAPE))
  {
    bool option_menu_bool{false};
    bool pause_screen_bool{ false };
    for (auto& screen : m_screen)
    {
      if (screen == option_screen)
      {
        m_screen.clear();
        m_screen.push_back(pause_screen);
        pause_screen_bool = true;
        break;
      }
      if (screen == control_screen)
      {
        m_screen.clear();
        m_screen.push_back(pause_screen);
        pause_screen_bool = true;
        break;
      }
     if (screen == main_screen)
      {
        m_screen.clear();
        m_screen.push_back(main_screen);
        break;
      }
      if (screen == level1_screen)
      {
        for (auto& screen2 : m_screen)
        {
          if (screen2 == pause_screen)
            pause_screen_bool = true;

          break;
        }
      }

      if (pause_screen_bool)
      {
          m_screen.clear();
          UnpauseGame();
          m_screen.push_back(level1_screen);
      }
      else
      {
          auto& p = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(UnitManager::GetPlayerID());
          PlayerCurrentPosition = p.position;
          Vector3 difference;
          difference = PlayerCurrentPosition - PlayerFirstPosition;
          PlayerFirstPosition = PlayerCurrentPosition;

          em.ForEach([&](UI& ui, EntityID& id)
              {
                  if (ui.screen == 0 || ui.screen == 4 || ui.screen == 5 || ui.screen == 7 || ui.screen == 8 || ui.screen == 10)
                      env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id).position += difference;
              });

          m_screen.push_back(pause_screen);
          pause_screen_bool = true;
          PauseGame();
      }
    }
  }

  if (InputManager::Instance().IsKeyTriggered(DEVK_U)) //Upgrade Page
  {
    bool upgrade_screen_exists = false;
    auto& p = env.pECS->GetWorld().GetEntityManager().GetComponent<Player>(UnitManager::GetPlayerID());
    for (auto& screen : m_screen)
    {
      if (screen == upgrade_page)
        upgrade_screen_exists = true;
    }
    
    if (upgrade_screen_exists)
    {
      m_screen.clear();
      m_screen.push_back(level1_screen);
    }
    else
    {
      m_screen.push_back(upgrade_page);

      if (p.UpgradedAtk)
          m_screen.push_back(upgraded_attack_only_page);
      if (p.UpgradedHP)
          m_screen.push_back(upgraded_health_only_page);
    }
  }
  for (auto& screen : m_screen)
  {
    if (screen == upgrade_page)
    {
      auto& p = env.pECS->GetWorld().GetEntityManager().GetComponent<EntityID>(UnitManager::GetPlayerID());
      Vector3 player_pos = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(p).position;
      
      em.ForEach([&](UI& ui, EntityID& id, EntityName& en)
      {
        if (en.name == "AttackFilled" || en.name == "AttackDefault")
          env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id).position = { player_pos.x + 1.194f, player_pos.y + 0.727f, 0.0f };
        else if (en.name == "HealthFilled" || en.name == "HealthDefault")
          env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id).position = { player_pos.x - 1.796f, player_pos.y + 0.727f, 0.0f };
        else
          env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id).position = { player_pos.x - 0.297f, player_pos.y + 1.013f, 0.0f };
      });
    }
  }

#ifdef DE_EDITOR
  auto &t = em.GetComponent<Transform>( { 0 } );
  float cameraWidth = Camera::allCameras[0]->Max( t ).x - Camera::allCameras[0]->Min( t ).x;
  float cameraHeight = Camera::allCameras[0]->Max( t ).y - Camera::allCameras[0]->Min( t ).y;
  float cursorViewPortDistanceX = InputManager::Instance().CurrentPosition().point_x - GamePanel::render_pos.x;
  float cursorViewPortDistanceY = InputManager::Instance().CurrentPosition().point_y - GamePanel::render_pos.y;
  auto p_x = ( ( cursorViewPortDistanceX / GamePanel::render_size.x ) * cameraWidth ) + Camera::allCameras[0]->Min( t ).x;
  auto p_y = Camera::allCameras[0]->Max( t ).y - ( ( cursorViewPortDistanceY / GamePanel::render_size.y ) * cameraHeight );
#endif
  if(!m_screen.empty())
	em.ForEach([&](UI& ui, Transform& t, Image& i, Renderer2D& r)
    {
      r.m_Active = false;
      
      
      for (auto screen : m_screen)
        if (screen == ui.screen)
        {
          r.m_Active = true;
          const bool rect_mouse = CollisionIntersection_RectMouse(t.position, i.GetWorldSize(), { p_x,p_y });
          
          if(screen == m_screen.back())
          {
            if (ui.ui_type == UIType::Button && rect_mouse)
            {
              // Animation update
              if (InputManager::Instance().IsKeyReleased(DEVK_LBUTTON))
              {
                if (!ui.functor_key.empty())
                { 
                  rttr::type::get<UISystem>().get_method(ui.functor_key.c_str()).invoke({ *this });
                  if (ui.overlay && ui.target_screen != -1)
                    m_screen.push_back(ui.target_screen);
                  //else if (ui.target_screen != -1)
                  //{
                  //  m_screen.clear();
                  //  m_screen.push_back(ui.target_screen);
                  //}
                }
                else if (ui.overlay && ui.target_screen != -1)
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

void UISystem::StartGame()
{
  JsonFile file;
  env.pECS->GetWorld().GetEntityManager().Clear();
  env.pECS->GetWorld().Load("World/MainLevelV2.json");
  env.pClock->TimeScale(1.0f);
  env.pECS->GetWorld().FindOrCreateSystem<EnemySpawner>().Initialize();
  m_screen.clear();
  auto& p = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(UnitManager::GetPlayerID());
  PlayerFirstPosition = { 1.0f, -1.01f, 0.0f };// p.position;
  m_screen.push_back(level1_screen);
}

void UISystem::QuitGame()
{
  env.pWin->Running(false);
}

void UISystem::BackToMainMenu()
{
  JsonFile file;
  env.pECS->GetWorld().GetEntityManager().Clear();
  env.pECS->GetWorld().Load("World/MainMenu.json");

  m_screen.clear();
  m_screen.push_back(main_screen);
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
  rttr::registration::class_<UISystem>( "UISystem" )
  .method( "Return", &UISystem::Return );

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

  rttr::registration::class_<UISystem>("QuitGame")
  .method("QuitGame", &UISystem::QuitGame);

  rttr::registration::class_<UISystem>("BackToMainMenu")
  .method("BackToMainMenu", &UISystem::BackToMainMenu);

  rttr::registration::class_<UISystem>("PauseGame")
  .method("PauseGame", &UISystem::PauseGame);
}
}