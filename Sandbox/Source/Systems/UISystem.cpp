#include "UISystem.h"


#include <rttr/registration.h>
#include <rttr/detail/registration/registration_impl.h>


#include "UnitManager.h"
#include "Components/Components.h"
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
const unsigned upgraded_page = 15;

void UISystem::Initialize()
{
    m_screen.push_back(level1_screen);

    //m_screen.push_back(pause_screen);
    //m_screen.push_back(main_screen);
    //m_screen.push_back(interface);
    //m_screen.push_back(control_screen);
    //m_screen.push_back(option_screen);
    //m_screen.push_back(credits_screen);
    //m_screen.push_back(gameover_screen);
    //m_screen.push_back(upgrade_page);
    //m_screen.push_back(level1_screen);
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
      bool main_game = false;
      bool pause_game_exists = false;
      for (auto screen : m_screen)
      {
          if (screen == pause_game_exists)
              pause_game_exists = true;
      }

      if (pause_game_exists)
          m_screen.pop_back();
      else
          m_screen.push_back(pause_screen);
  }

  if (InputManager::Instance().IsKeyTriggered(DEVK_U)) //Upgrade Page
  {
      bool upgrade_screen_exists = false;
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
          if (screen == m_screen.back() &&
              ui.ui_type == UIType::Button &&
              CollisionIntersection_RectMouse(t.position, i.GetWorldSize(), { p_x,p_y }))
                  {
                      // Animation update

                      if (InputManager::Instance().IsKeyReleased(DEVK_LBUTTON))
                      {
                          if (!ui.functor_key.empty())
                              rttr::type::get<UISystem>().get_method(ui.functor_key.c_str()).invoke({ *this });
                          else if (ui.overlay && ui.target_screen != -1)
                              m_screen.push_back(ui.target_screen);
                          else if (ui.target_screen != -1)
                          {
                              m_screen.clear();
                              m_screen.push_back(ui.target_screen);
                          }
                      }
                  }
          else if (screen == m_screen.back() &&
              ui.ui_type == UIType::Interface &&
              CollisionIntersection_RectMouse(t.position, i.GetWorldSize(), { p_x,p_y }))
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
                  
          else if (screen == m_screen.back() &&
              ui.ui_type == UIType::Button &&
              !(CollisionIntersection_RectMouse(t.position, i.GetWorldSize(), { p_x,p_y })))
                      {
                          // Animation update

                          if (ui.overlay && ui.previous_screen != -1)
                              m_screen.push_back(ui.previous_screen);
                          else if (ui.previous_screen != -1)
                          {
                              m_screen.clear();
                              m_screen.push_back(ui.previous_screen);
                          }
                      }
          
        }
    });

}

void UISystem::Return()
{
  m_screen.pop_back();
  std::cout << "returning" << std::endl;
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

  m_screen.clear();
  m_screen.push_back(level1_screen);
}

void UISystem::QuitGame()
{
  env.pECS->GetWorld().GetEntityManager().Clear();
  env.pECS->GetWorld().ShutdownSystems();
  env.pWin->Shutdown();
}

void UISystem::PauseGame()
{
  std::cout << "pausing game" << std::endl;
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
	
  rttr::registration::class_<UISystem>("StartGame")
  .method("StartGame", &UISystem::StartGame);
}
}