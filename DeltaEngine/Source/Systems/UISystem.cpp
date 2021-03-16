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

#include <vector>
#include <rttr/registration.h>
#include <rttr/detail/registration/registration_impl.h>

#include "ImGui/Panels/GamePanel.h"
#include "Input/InputManager.h"
#include "Input/Keys.h"

namespace DeltaEngine
{

std::vector<unsigned> screens;
unsigned transitioning_screen;
bool end { true };

bool GUI_Collision( Image &i, RendererOverlay &r, Camera &c, float &t_aspect, float &cameraWidth, float &cameraHeight, float &p_x, float &p_y )
{
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
    return true;
  return false;
}

void InvokeFunc( const std::string &name )
{
  if ( !name.empty() )
    rttr::type::get_global_method( name ).invoke( {} );
}

void UISystem::Initialize()
{
  screens.push_back( 0 );
}

void UISystem::Update()
{

}

void UISystem::LateUpdate()
{

  if ( transitioning_screen )
  {
    GetEnv().pECS->GetWorld().GetEntityManager().ForEach( [&]( GUI &gui, RendererOverlay &o )
    {
      if ( transitioning_screen == gui.screen )
      {
        o.m_Color.a = Math::Clamp01( o.m_Color.a - 0.02f );
        if ( o.m_Color.a == 0.0f )
        {
          o.m_Active = false;
          end = true;
        }
      }
    } );
  }

  if ( !end )
    return;
  else
    transitioning_screen = 0;

#ifdef DE_EDITOR
  auto cameraWidth = GamePanel::render_size.x;
  auto cameraHeight = GamePanel::render_size.y;
  auto p_x = InputManager::Instance().CurrentPosition().point_x - GamePanel::render_pos.x;
  auto p_y = InputManager::Instance().CurrentPosition().point_y - GamePanel::render_pos.y;
  auto t_aspect = GamePanel::render_size.x / GamePanel::render_size.y;

#else
  auto cameraWidth = static_cast<float>(GetEnv().pWin->Width());
  auto cameraHeight = static_cast<float>(GetEnv().pWin->Height());
  auto p_x = InputManager::Instance().CurrentPosition().point_x - GetEnv().pWin->ClientTopLeft().point_x;
  auto p_y = InputManager::Instance().CurrentPosition().point_y - GetEnv().pWin->ClientTopLeft().point_y;
  auto t_aspect = 1.0f * cameraWidth / cameraHeight;

#endif

  Camera &c = *Camera::allCameras[0];

  Query q;

  std::vector<unsigned> canvas_ids;

  q.Exclude<Button, Toggle, Slider>();

  // Query Canvas
  em.ForEach( q, [&]( EntityID &id, GUI &gui )
  {
    for ( auto screen : screens )
      if ( gui.type == GUIType::Canvas && gui.screen == screen )
        canvas_ids.push_back( id.index );
  } );

  if ( canvas_ids.empty() )
    return;

  std::sort( canvas_ids.begin(), canvas_ids.end() );

  // Set to render
  for ( auto id : canvas_ids )
  {
    auto childrens = em.GetChildrens( id );
    if ( em.HasComponent<RendererOverlay>( { id } ) )
    {
      auto &p_render = em.GetComponent<RendererOverlay>( { id } );
      p_render.m_Active = true;
      p_render.m_Color.a = Math::Clamp01( p_render.m_Color.a + 0.02f );
    }
    for ( auto child : childrens )
    {
      auto &render = em.GetComponent<RendererOverlay>( { child } );
      render.m_Active = true;
      render.m_Color.a = Math::Clamp01( render.m_Color.a + 0.02f );
    }

  }

  // Check Interactable
  auto top_childrens = em.GetChildrens( canvas_ids.back() );
  for ( auto child : top_childrens )
  {
    switch ( em.GetComponent<GUI>( { child } ).type )
    {
      case GUIType::Button:
      {
        auto &button = em.GetComponent<Button>( { child } );
        auto &state = em.GetComponent<State>( { child } );
        if ( GUI_Collision( em.GetComponent<Image>( { child } ), em.GetComponent<RendererOverlay>( { child } ), c, t_aspect, cameraWidth, cameraHeight, p_x, p_y ) )
        {
          if ( !state.GetBool( "Hover" ) )
          {
            state.SetBool( "Hover", true );
            InvokeFunc( button.on_hover );
            return;
          }
          if ( InputManager::Instance().IsMouseReleased( DEVK_LBUTTON ) )
          {
            state.SetBool( "Hover", false );
            InvokeFunc( button.on_click );
            return;
          }

        }
        else
        {
          if ( state.GetBool( "Hover" ) )
          {
            state.SetBool( "Hover", false );
            InvokeFunc( button.on_exit );
            return;
          }
        }
        break;
      }
      case GUIType::Toggle:
      {
        auto &toggle = em.GetComponent<Toggle>( { child } );

        break;
      }
      case GUIType::Slider:
      {
        auto &slider = em.GetComponent<Slider>( { child } );

        break;
      }

      default:
        break;
    }
  }
}

void UISystem::PopScreen()
{
  transitioning_screen = screens.back();
  screens.pop_back();
  end = false;
}

void UISystem::PushScreen( unsigned screen )
{
  screens.push_back( screen );
  GetEnv().pECS->GetWorld().GetEntityManager().ForEach( [&]( GUI &g, RendererOverlay &o )
  {
    if ( g.screen == screen )
      o.m_Color.a = 0.0f;
  } );
}

void UISystem::ClearScreens()
{
  screens.clear();
  screens.push_back( 0 );
}

unsigned UISystem::CurrentScreen()
{
  return screens.back();
}

}