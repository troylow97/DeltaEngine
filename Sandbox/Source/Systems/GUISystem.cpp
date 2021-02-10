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
#include "GUISystem.h"

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
#include "Systems/OCullSystem.h"

namespace DeltaEngine
{

void GUISystem::Initialize()
{

}



void GUISystem::Update()
{}

void GUISystem::LateUpdate()
{
  auto cameraWidth = GamePanel::render_size.x;
  auto cameraHeight = GamePanel::render_size.y;

#ifdef DE_EDITOR
  auto p_x = InputManager::Instance().CurrentPosition().point_x - GamePanel::render_pos.x;
  auto p_y = InputManager::Instance().CurrentPosition().point_y - GamePanel::render_pos.y;
#else
  auto p_x= InputManager::Instance().CurrentPosition().point_x - GetEnv().pWin->ClientTopLeft().point_x;
  auto p_y = InputManager::Instance().CurrentPosition().point_y - GetEnv().pWin->ClientTopLeft().point_y;
#endif

  auto width = Camera::allCameras[0]->GetTrueViewportSize();
  auto height = width / Camera::allCameras[0]->GetAspectRatio();

  Camera &c = *Camera::allCameras[0];
  auto t_aspect = GamePanel::render_size.x / GamePanel::render_size.y;

  em.ForEach( [&]( RendererOverlay &r, Image &i )
  {
    auto coords = r.GetScreenspaceBounds( i );
    if ( c.GetFixedAspectRatio() > c.GetAspectRatio() )
    {
      coords.y = coords.y * ( c.GetFixedAspectRatio() / t_aspect ) + (( 1 - c.GetFixedAspectRatio() / c.GetAspectRatio() ) / 2);
      coords.w = coords.w * ( c.GetFixedAspectRatio() / t_aspect ) + (( 1 - c.GetFixedAspectRatio() / c.GetAspectRatio() ) / 2);
    }
    else
    {
      coords.x = coords.x * ( c.GetFixedAspectRatio() / t_aspect ) + (( 1 - c.GetFixedAspectRatio() / c.GetAspectRatio() ) / 2);
      coords.z = coords.z * ( c.GetFixedAspectRatio() / t_aspect ) + (( 1 - c.GetFixedAspectRatio() / c.GetAspectRatio() ) / 2);
    }
    coords.x *= width;
    coords.y *= height;
    coords.z *= width;
    coords.w *= height;

    DeltaEngine_CORE_INFO( "Coords Min: {},{}    Max: {},{}", coords.x, coords.y, coords.z, coords.w );
    DeltaEngine_CORE_INFO( "Mouse: {},{}", p_x, p_y );

    DeltaEngine_CORE_INFO( "width: {}, height: {}", width, height );
  } );

  //( 1 - c.GetFixedAspectRatio() / c.GetAspectRatio() ) / 2;

  //if ( c.GetFixedAspectRatio() / c.GetAspectRatio() )
  //{
  //  coords
  //}



}



//RTTR_REGISTRATION
//{
//  rttr::registration::class_<UISystem>("UISystem")
//    .method("Return", &UISystem::Return);

//  rttr::registration::class_<UISystem>("UpgradeDamageButton")
//    .method("UpgradeDamageButton", &UISystem::UpgradeDamageButton);

//  rttr::registration::class_<UISystem>("UpgradeHPButton")
//    .method("UpgradeHPButton", &UISystem::UpgradeHPButton);

//  rttr::registration::class_<UISystem>("PauseGame")
//    .method("PauseGame", &UISystem::PauseGame);

//  rttr::registration::class_<UISystem>("UnpauseGame")
//    .method("UnpauseGame", &UISystem::UnpauseGame);

//  rttr::registration::class_<UISystem>("StartGame")
//    .method("StartGame", &UISystem::StartGame);

//  rttr::registration::class_<UISystem>("RestartGame")
//    .method("RestartGame", &UISystem::RestartGame);

//  rttr::registration::class_<UISystem>("QuitGame")
//    .method("QuitGame", &UISystem::QuitGame);

//  rttr::registration::class_<UISystem>("BackToMainMenu")
//    .method("BackToMainMenu", &UISystem::BackToMainMenu);

//  rttr::registration::class_<UISystem>("PauseGame")
//    .method("PauseGame", &UISystem::PauseGame);
//}
}
