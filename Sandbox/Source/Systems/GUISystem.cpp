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
  auto p_x = InputManager::Instance().CurrentPosition().point_x - GetEnv().pWin->ClientTopLeft().point_x;
  auto p_y = InputManager::Instance().CurrentPosition().point_y - GetEnv().pWin->ClientTopLeft().point_y;
#endif
  auto t_aspect = GamePanel::render_size.x / GamePanel::render_size.y;

  auto width = Camera::allCameras[0]->GetTrueViewportSize();
  auto height = width / t_aspect;

  Camera &c = *Camera::allCameras[0];

  em.ForEach( [&]( RendererOverlay &r, Image &i )
  {
    auto coords = r.GetScreenspaceBounds( i );
    float zeroPos = ( 1 - c.GetFixedAspectRatio() / t_aspect ) / 2;
    if ( c.GetFixedAspectRatio() > c.GetAspectRatio() )
    {
      float halfRatioY = coords.y < 0.5f ? coords.y * 2 : ( coords.y - 0.5f ) * 2;
      float halfRatioW = coords.w < 0.5f ? coords.w * 2 : ( coords.w - 0.5f ) * 2;

      std::cerr << coords.y << ", " << coords.w << std::endl;
      std::cerr << c.GetFixedAspectRatio() << ", " << t_aspect << std::endl;

      coords.y = coords.y < 0.5f ? zeroPos + ( 0.5f - zeroPos ) * halfRatioY : 1 - zeroPos - ( 0.5f - zeroPos ) * halfRatioY;
      coords.w = coords.w < 0.5f ? zeroPos + ( 0.5f - zeroPos ) * halfRatioW : 1 - zeroPos - ( 0.5f - zeroPos ) * halfRatioW;

      std::cerr << coords.y << ", " << coords.w << std::endl;
    }
    else
    {
      float halfRatioX = coords.x < 0.5f ? coords.x * 2 : ( 1 - coords.x ) * 2;
      float halfRatioZ = coords.z < 0.5f ? coords.z * 2 : ( 1 - coords.z ) * 2;

      std::cerr << coords.x << ", " << coords.z << std::endl;
      std::cerr << c.GetFixedAspectRatio() << ", " << t_aspect << std::endl;

      coords.x = coords.x < 0.5f ? zeroPos + ( 0.5f - zeroPos ) * halfRatioX : 1 - zeroPos - ( 0.5f - zeroPos ) * halfRatioX;
      coords.z = coords.z < 0.5f ? zeroPos + ( 0.5f - zeroPos ) * halfRatioZ : 1 - zeroPos - ( 0.5f - zeroPos ) * halfRatioZ;

      std::cerr << coords.x << ", " << coords.z << std::endl;
    }
    coords.x *= width;
    coords.y *= height;
    coords.z *= width;
    coords.w *= height;

    if ( CollisionIntersection_RectMinMaxMouse( { coords.x, coords.y }, { coords.z, coords.w }, { p_x, p_y } ) )
    {
      DeltaEngine_CORE_INFO( "Coords Min: {},{}    Max: {},{}", coords.x, coords.y, coords.z, coords.w );
      DeltaEngine_CORE_INFO( "Mouse: {},{}", p_x, p_y );

      DeltaEngine_CORE_INFO( "width: {}, height: {}", width, height );
    }
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
