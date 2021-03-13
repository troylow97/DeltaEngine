/**********************************************************************************
* \file   ButtonsPanel.cpp
* \brief  The file contains implementation of ButtonsPanel.
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "ImGui/Panels/ButtonsPanel.h"
#include "ImGui/IconsFontAwesome5.h"
#include "Input/InputManager.h"

#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"
#include "ImGui/Editor.h"

namespace DeltaEngine
{
ButtonsPanel::ButtonsPanel( std::string str, Editor &e ) :
  IPanel( str, e )
{
  m_enabled = true;
}

ButtonsPanel::~ButtonsPanel()
{
  m_enabled = false;
}

void ButtonsPanel::Render()
{
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse/* | ImGuiWindowFlags_NoSavedSettings*/;
  ImGui::Begin( m_name.c_str(), &m_enabled, window_flags );

  ImVec2 tools_pos = { ImGui::GetWindowSize().x * 0.03f, ImGui::GetWindowSize().y * 0.1f };
  ImVec2 simulation_pos = { ImGui::GetWindowSize().x * 0.45f, ImGui::GetWindowSize().y * 0.1f };

  float lineHeight = ImGui::GetIO().FontDefault->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;
  ImVec2 buttonSize = { lineHeight, lineHeight };

  const float original = Editor::font_awesome->Scale;
  Editor::font_awesome->Scale = 0.5f;
  ImGui::PushFont( Editor::font_awesome );

  ImGui::SetCursorPos( tools_pos );
  if ( ImGui::Button( ICON_FA_HAND_PAPER, buttonSize ) ) // ICON_FA_ARROWS_ALT
    Editor::tool_selection = Editor::Tool::Camera;
  ImGui::SameLine();
  if ( ImGui::Button( ICON_FA_ARROWS_ALT, buttonSize ) )
    Editor::tool_selection = Editor::Tool::EntitySelector;

  Editor::font_awesome->Scale = 0.4f;
  ImGui::PushFont( Editor::font_awesome );

  ImGui::SetCursorPos( simulation_pos );
  if ( ImGui::Button( ICON_FA_PLAY, buttonSize ) )
  {
    if ( !Editor::simulation_running )
    {
      Editor::simulation_running = true;
      GetEnv().pECS->GetWorld().Save( Editor::mounted_path );
    }
    GetEnv().pECS->GetWorld().SetPause( false );
  }
  ImGui::SameLine();
  if ( ImGui::Button( ICON_FA_PAUSE, buttonSize ) )
  {
    if ( Editor::simulation_running )
      GetEnv().pECS->GetWorld().SetPause( true );
  }
  ImGui::SameLine();
  if ( ImGui::Button( ICON_FA_STOP, buttonSize ) )
  {
    if ( Editor::simulation_running )
    {
      Editor::simulation_running = false;
      GetEnv().pECS->GetWorld().SetPause( true );
      GetEnv().pECS->GetWorld().GetEntityManager().Clear();
      GetEnv().pECS->GetWorld().Load( Editor::mounted_path );
    }
  }

  ImGui::PopFont();
  ImGui::PopFont();
  Editor::font_awesome->Scale = original;

  ImGui::End();
}
}
