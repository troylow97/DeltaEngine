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
#include "Audio/AudioEngine.h"

namespace DeltaEngine
{

// Variables
std::set<unsigned> screens;
unsigned transitioning_screen;
bool end { true };
size_t e_id { u64_max };
std::string on_click {};

// Forward Declaration Function
bool InvokeFunc( const std::string &name );

template <typename... T>
bool InvokeFunc( const std::string &name, T&... params );
bool GUI_Collision( Image &i, RendererOverlay &r, Camera &c, float &t_aspect, float &cameraWidth, float &cameraHeight, float &p_x, float &p_y, GUIType type );
Vector2 GUI_LeftRightLimit( Image &i, RendererOverlay &r, Camera &c, float &t_aspect, float &cameraWidth, float &cameraHeight );
Vector2 GUI_MouseToOverlay(float p_x, float p_y, RendererOverlay& r, float& cameraWidth, float& cameraHeight);

void UISystem::Initialize()
{
  screens.insert( 0 );
}

void UISystem::Update()
{

#ifdef DE_EDITOR
  auto cameraWidth = GamePanel::render_size.x;
  auto cameraHeight = GamePanel::render_size.y;
  auto p_x = InputManager::Instance().CurrentPosition().point_x - GamePanel::render_pos.x;
  auto p_y = InputManager::Instance().CurrentPosition().point_y - GamePanel::render_pos.y;
#else
  auto cameraWidth = static_cast<float>( GetEnv().pWin->Width() );
  auto cameraHeight = static_cast<float>( GetEnv().pWin->Height() );
  auto p_x = InputManager::Instance().CurrentPosition().point_x - GetEnv().pWin->ClientTopLeft().point_x;
  auto p_y = InputManager::Instance().CurrentPosition().point_y - GetEnv().pWin->ClientTopLeft().point_y;
#endif

  em.ForEach( [&]( GUI &gui, RendererOverlay &o, Cursor &c )
  {
    o.m_SortingLayer = 100;
    o.m_Active = c.visible;
    o.pos = { p_x / cameraWidth * 1920.0f ,-p_y / cameraHeight * 1080.0f };
  } );
}

void UISystem::LateUpdate()
{

  if ( transitioning_screen )
  {
    em.ForEach( [&]( GUI &gui, RendererOverlay &o )
    {
      if ( transitioning_screen == gui.screen )
      {
        if ( gui.type == GUIType::Canvas )
          o.m_Active = false;
        else
        {
          o.m_Color.a = Math::Clamp01( o.m_Color.a - 0.04f );
          if ( o.m_Color.a == 0.0f )
          {
            o.m_Active = false;
            end = true;
          }
        }
      }
    } );

    if ( !end )
      return;
    else
      transitioning_screen = 0;
  }

  if ( e_id != u64_max )
  {
    if ( em.HasComponent<Animator>( { e_id } ) )
    {
      if ( em.GetComponent<Animator>( { e_id } ).m_ControllerKey != "Animation/EmptyButton" && em.GetComponent<Animator>( { e_id } ).LoopsCompleted() )
      {
        em.GetComponent<State>( { e_id } ).SetBool( "Click", false );
        e_id = u64_max;
        InvokeFunc( on_click );
        on_click = {};
      }
    }
    else
    {
      em.GetComponent<State>( { e_id } ).SetBool( "Click", false );
      e_id = u64_max;
      InvokeFunc( on_click );
      on_click = {};
    }
    return;
  }

#ifdef DE_EDITOR
  auto cameraWidth = GamePanel::render_size.x;
  auto cameraHeight = GamePanel::render_size.y;
  auto p_x = InputManager::Instance().CurrentPosition().point_x - GamePanel::render_pos.x;
  auto p_y = InputManager::Instance().CurrentPosition().point_y - GamePanel::render_pos.y;
  auto t_aspect = GamePanel::render_size.x / GamePanel::render_size.y;

#else
  auto cameraWidth = static_cast<float>( GetEnv().pWin->Width() );
  auto cameraHeight = static_cast<float>( GetEnv().pWin->Height() );
  auto p_x = InputManager::Instance().CurrentPosition().point_x - GetEnv().pWin->ClientTopLeft().point_x;
  auto p_y = InputManager::Instance().CurrentPosition().point_y - GetEnv().pWin->ClientTopLeft().point_y;
  auto t_aspect = 1.0f * cameraWidth / cameraHeight;
#endif

  Camera &c = *Camera::allCameras[0];

  Query q;

  std::set<size_t> canvas_ids;

  q.Exclude<Button, Toggle, Slider>();

  // Query Canvas
  em.ForEach( q, [&]( EntityID &id, GUI &gui )
  {
    for ( auto screen : screens )
      if ( gui.type == GUIType::Canvas && gui.screen == screen )
        canvas_ids.insert( id.index );
  } );

  if ( canvas_ids.empty() )
    return;

  // Set to render
  for ( auto id : canvas_ids )
  {
    auto childrens = em.GetChildrens( id );
    if ( em.HasComponent<RendererOverlay>( { id } ) )
      em.GetComponent<RendererOverlay>( { id } ).m_Active = true;

    for ( auto child : childrens )
    {
      auto &render = em.GetComponent<RendererOverlay>( { child } );
      render.m_Active = true;
      render.m_Color.a = Math::Clamp01( render.m_Color.a + 0.04f );
    }

  }

  auto top_parent = std::prev( canvas_ids.end() );
  auto top_childrens = em.GetChildrens( *top_parent );
  for ( auto it = top_childrens.rbegin(); it != top_childrens.rend(); ++it )
  {
    auto &child = *it;
    switch ( em.GetComponent<GUI>( { child } ).type )
    {
      case GUIType::Button:
      {
        auto &button = em.GetComponent<Button>( { child } );
        auto &state = em.GetComponent<State>( { child } );
        if ( GUI_Collision( em.GetComponent<Image>( { child } ), em.GetComponent<RendererOverlay>( { child } ), c, t_aspect, cameraWidth, cameraHeight, p_x, p_y , GUIType::Button) )
        {
          if ( !state.GetBool( "Hover" ) )
          {
            state.SetBool( "Hover", true );
            InvokeFunc( button.on_hover );
            return;
          }
          if ( InputManager::Instance().IsMouseReleased( DEVK_LBUTTON ) )
          {
            if ( !button.on_click.empty() )
            {
              AudioEngine::SetEventVolume( AudioEngine::Play2DEvent( "event:/UI Sounds/Button Click" ), 0.4f );

              if ( em.HasComponent<Animator>( { child } ) )
                em.GetComponent<Animator>( { child } ).m_LoopsCompleted = 0;

              state.SetBool( "Hover", false );
              state.SetBool( "Click", true );
              e_id = child;
              on_click = button.on_click;
            }
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
        //auto &toggle = em.GetComponent<Toggle>( { child } );
        break;
      }
      case GUIType::Slider:
      {
        auto &slider = em.GetComponent<Slider>( { child } );

        auto left_right_limit = GUI_LeftRightLimit( em.GetComponent<Image>( { child } ), em.GetComponent<RendererOverlay>( { child } ), c, t_aspect, cameraWidth, cameraHeight );
        auto &e_handle_render = em.GetComponent<RendererOverlay>( { slider.handle_entity } );
        auto &e_handle_image = em.GetComponent<Image>( { slider.handle_entity } );

        slider.value = Math::Clamp01(slider.value);
        e_handle_render.pos.x = left_right_limit.x + (left_right_limit.y - left_right_limit.x) * slider.value;
        auto &e_fill_image = em.GetComponent<Image>( { slider.fill_entity } );
        e_fill_image.m_FillAmount = slider.value;

        if ( !slider.selected )
        {
          if ( GUI_Collision( em.GetComponent<Image>( { child } ), em.GetComponent<RendererOverlay>( { child } ), c, t_aspect, cameraWidth, cameraHeight, p_x, p_y, GUIType::Slider ) ||
               GUI_Collision( e_handle_image, e_handle_render, c, t_aspect, cameraWidth, cameraHeight, p_x, p_y, GUIType::Slider) )
            if ( InputManager::Instance().IsMousePressed( DEVK_LBUTTON ) )
              slider.selected = true;
        }
        if ( slider.selected )
        {
          if(InputManager::Instance().IsMousePressed(DEVK_LBUTTON))
          {
            auto mospos = GUI_MouseToOverlay(p_x, p_y, em.GetComponent<RendererOverlay>({ child }), cameraWidth, cameraHeight);
            auto val = Math::Clamp01((mospos.x - left_right_limit.x) / (left_right_limit.y - left_right_limit.x));
            if ( val <= slider.value || val >= slider.value )
            {
              slider.value = val;
              InvokeFunc( slider.on_change, slider.value );
              return;
            }
          }
          else if ( InputManager::Instance().IsMouseReleased( DEVK_LBUTTON ) )
          {
            slider.selected = false;
            on_click.clear();
            return;
          }
        }

        break;
      }

      default:
        break;
    }
  }
}

void UISystem::PopScreen()
{
  if ( auto screen = std::prev( screens.end() ); *screen )
  {
    transitioning_screen = *screen;
    screens.erase( screen );
    end = false;
  }
}

void UISystem::PushScreen( unsigned screen )
{
  auto [it, result] = screens.insert( screen );
  if ( result )
    GetEnv().pECS->GetWorld().GetEntityManager().ForEach( [&]( GUI &g, RendererOverlay &o )
  {
    if ( g.screen == screen && g.type != GUIType::Canvas )
      o.m_Color.a = 0.0f;
  } );
}

void UISystem::ClearScreens()
{
  screens.clear();
  screens.insert( 0 );
}

unsigned UISystem::CurrentScreen()
{
  auto it = std::prev( screens.end() );
  return *it;
}

// Global Func
bool InvokeFunc( const std::string &name )
{
  if ( name.empty() )
    return false;
  rttr::type::get_global_method( name ).invoke( {} );
  return true;
}

template <typename... T>
bool InvokeFunc(const std::string &name, T&... params )
{
  if ( name.empty() )
    return false;
  rttr::type::get_global_method( name ).invoke( {}, params... );
  return true;
}

bool GUI_Collision( Image &i, RendererOverlay &r, Camera &c, float &t_aspect, float &cameraWidth, float &cameraHeight, float &p_x, float &p_y, GUIType type )
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

  auto x_size { ( coords.z - coords.x )  }; 
  auto y_size { ( coords.w - coords.y ) };

  if (type != GUIType::Slider)
  {
    x_size *= 0.25f;
    y_size *= 0.25f;
  }
  else
  {
    x_size *= 0.001f;
    y_size *= 0.001f;
  }

  if (type == GUIType::Slider)
    std::cerr << CollisionIntersection_RectMinMaxMouse({ coords.x + x_size , coords.y + y_size }, { coords.z - x_size, coords.w - y_size }, { p_x, p_y }) << std::endl;

  if ( CollisionIntersection_RectMinMaxMouse( { coords.x + x_size , coords.y + y_size }, { coords.z - x_size, coords.w - y_size }, { p_x, p_y } ) )
    return true;
  return false;
}

Vector2 GUI_LeftRightLimit( Image &i, RendererOverlay &r, Camera &c, float &t_aspect, float &cameraWidth, float &cameraHeight )
{
  return { r.pos.x - r.size.x / 2, r.pos.x + r.size.x / 2 };
}
Vector2 GUI_MouseToOverlay(float p_x, float p_y, RendererOverlay& r, float& cameraWidth, float& cameraHeight)
{
  p_x = p_x / cameraWidth * r.refRes.x - r.refRes.x / 2;
  p_y = p_y / cameraHeight * r.refRes.y - r.refRes.y / 2;
  return Vector2(p_x, p_y);
}

}