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


#include <vector>
#include <rttr/registration.h>
#include <rttr/detail/registration/registration_impl.h>


#include "UISystem.h"
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

std::vector<unsigned> current;
bool s { false };
bool menu { false };
bool change { false };
std::string tmp {};


void GUISystem::PopScreen()
{
  if ( current.size() > 1 )
  {
    em.ForEach( [&]( GUI &gui, RendererOverlay &r )
    {
      if ( gui.screen == current.back() )
        r.m_Active = false;
    } );
    current.pop_back();
  }
}


void GUISystem::PushScreen( unsigned screen )
{
  current.push_back( screen );
  em.ForEach( [&]( GUI &gui, RendererOverlay &r )
  {
    if ( gui.screen == screen )
      r.m_Active = true;
  } );
  s = true;
}

void GUISystem::Initialize()
{
  current.push_back( 0 );
}

void GUISystem::Update()
{}

void GUISystem::LateUpdate()
{


#ifdef DE_EDITOR
  auto cameraWidth = GamePanel::render_size.x;
  auto cameraHeight = GamePanel::render_size.y;
  auto p_x = InputManager::Instance().CurrentPosition().point_x - GamePanel::render_pos.x;
  auto p_y = InputManager::Instance().CurrentPosition().point_y - GamePanel::render_pos.y;
  auto t_aspect = GamePanel::render_size.x / GamePanel::render_size.y;

#else
  auto cameraWidth = GetEnv().pWin->Width();
  auto cameraHeight = GetEnv().pWin->Height();
  auto p_x = InputManager::Instance().CurrentPosition().point_x - GetEnv().pWin->ClientTopLeft().point_x;
  auto p_y = InputManager::Instance().CurrentPosition().point_y - GetEnv().pWin->ClientTopLeft().point_y;
  auto t_aspect = 1.0f * cameraWidth / cameraHeight;

#endif

  auto width = Camera::allCameras[0]->GetTrueViewportSize();
  auto height = width / t_aspect;

  Camera &c = *Camera::allCameras[0];

  em.ForEach( [&]( EntityID &id, EntityName &name, RendererOverlay &r, Image &i, GUI &gui )
  {
    if ( !name.name.compare( "MenuTitle" ))
      menu = true;

    if ( s )
      return;

    if ( !r.m_Active || gui.screen != current.back() )
      return;

    auto coords = r.GetScreenspaceBounds( i );
    if ( c.GetFixedAspectRatio() > c.GetAspectRatio() )
    {
      float zeroPos = ( 1 - t_aspect / c.GetFixedAspectRatio() ) / 2;
      float halfRatioY = coords.y < 0.5f ? coords.y * 2 : ( 1 - coords.y ) * 2;
      float halfRatioW = coords.w < 0.5f ? coords.w * 2 : ( 1 - coords.w ) * 2;

      coords.y = coords.y < 0.5f ? zeroPos + ( 0.5f - zeroPos ) * halfRatioY : 1 - zeroPos - ( 0.5f - zeroPos ) * halfRatioY;
      coords.w = coords.w < 0.5f ? zeroPos + ( 0.5f - zeroPos ) * halfRatioW : 1 - zeroPos - ( 0.5f - zeroPos ) * halfRatioW;
    }
    else
    {
      float zeroPos = ( 1 - c.GetFixedAspectRatio() / t_aspect ) / 2;
      float halfRatioX = coords.x < 0.5f ? coords.x * 2 : ( 1 - coords.x ) * 2;
      float halfRatioZ = coords.z < 0.5f ? coords.z * 2 : ( 1 - coords.z ) * 2;

      coords.x = coords.x < 0.5f ? zeroPos + ( 0.5f - zeroPos ) * halfRatioX : 1 - zeroPos - ( 0.5f - zeroPos ) * halfRatioX;
      coords.z = coords.z < 0.5f ? zeroPos + ( 0.5f - zeroPos ) * halfRatioZ : 1 - zeroPos - ( 0.5f - zeroPos ) * halfRatioZ;
    }
    coords.x *= cameraWidth;
    coords.y *= cameraHeight;
    coords.z *= cameraWidth;
    coords.w *= cameraHeight;

    if ( CollisionIntersection_RectMinMaxMouse( { coords.x, coords.y }, { coords.z, coords.w }, { p_x, p_y } ) )
    {
      if ( em.HasComponent<State>( id ) )
      {
        auto& s = em.GetComponent<State>( id );
        s.SetBool( "Hover", true );
      }

      if ( InputManager::Instance().IsMouseReleased( DEVK_LBUTTON ) )
      {
        if ( gui.type == GUIType::Button )
        {
          if ( !gui.func.empty() )
          {
            if ( gui.func == "Start" )
              rttr::type::get<GUISystem>().get_method( gui.func ).invoke( { *this }, gui.file );
            else
              rttr::type::get<GUISystem>().get_method( gui.func ).invoke( { *this } );
          }
          else
            PushScreen( gui.target );
        }
      }
    }
    else
    {
      if ( em.HasComponent<State>( id ) )
      {
        auto& s = em.GetComponent<State>( id );
        s.SetBool( "Hover", false );
      }
    }

  } );

  if ( InputManager::Instance().IsKeyReleased( DEVK_ESCAPE ) )
  {
    if (!menu )
    {
      if ( env.pClock->TimeScale() > 0.1f )
        Pause();
      else if ( current.back() == 0 )
        Unpause();
      else
        PopScreen();
    }
    else if ( current.back() != 0 )
      PopScreen();
  }

    

  s = false;
  menu = false;

  if ( change )
    ChangeState();

}

void GUISystem::ChangeState()
{
  env.pECS->GetWorld().GetEntityManager().Clear();
  env.pECS->GetWorld().Load( tmp );
  current.clear();
  PushScreen( 0 );
  tmp.clear();
  change = false;
}



void GUISystem::Pause()
{
  env.pClock->TimeScale( 0.0f );
  PushScreen( 1 );
}

void GUISystem::Unpause()
{
  env.pClock->TimeScale( 1.0f );
  PopScreen();
}

void GUISystem::Start( const std::string &file )
{
  tmp.assign( file );
  change = true;
}


void GUISystem::Quit()
{
  env.pECS->GetWorld().FindOrCreateSystem<EnemySpawner>().Shutdown();
  env.pWin->Running( false );
}

RTTR_REGISTRATION
{
  rttr::registration::class_<GUISystem>( "GUISystem" )
    .method( "Pause", &GUISystem::Pause )
    .method( "Unpause", &GUISystem::Unpause )
    .method( "Start", &GUISystem::Start )
    .method( "Quit", &GUISystem::Quit )
    .method("PopScreen", &GUISystem::PopScreen);
}
}