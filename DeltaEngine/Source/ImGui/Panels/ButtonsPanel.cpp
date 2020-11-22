#include "ImGui/Panels/ButtonsPanel.h"
#include "ImGui/IconsFontAwesome5.h"
#include "Input/InputManager.h"

#include "Core/GlobalStruct.h"
//#include "Render/Texture.h"
//#include "Assets/AssetManager.h"
//#include "ECS/ECSModule.h"
//#include "Core/Utils/FileUtils.h"
#include "ECS/ECSModule.h"
#include "ImGui/Editor.h"

namespace DeltaEngine
{
  ButtonsPanel::ButtonsPanel(std::string str) :
    IPanel(str)
  {
    m_enabled = true;
  }

  ButtonsPanel::~ButtonsPanel()
  {
    m_enabled = false;
  }

  bool ButtonsPanel::DraggedFileIn()
  {
    if (InputManager::Instance().CurrentPosition().point_x >= GetTopLeft().x && InputManager::Instance().
      CurrentPosition().point_x <= GetBottomRight().x
      && InputManager::Instance().CurrentPosition().point_y >= GetTopLeft().y && InputManager::Instance().
      CurrentPosition().point_y <= GetBottomRight().y)
    {
      std::cout << "it is in button panel!!!" << std::endl;
      return true;
    }
    return false;
  }

  void ButtonsPanel::Render(bool isdragged)
  {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse/* | ImGuiWindowFlags_NoSavedSettings*/;
    ImGui::Begin(m_name.c_str(), &m_enabled, window_flags);

    topLeft = ImGui::GetWindowContentRegionMin();
    bottomRight = ImGui::GetWindowContentRegionMax();

    topLeft.x += ImGui::GetWindowPos().x;
    topLeft.y += ImGui::GetWindowPos().y;
    bottomRight.x += ImGui::GetWindowPos().x;
    bottomRight.y += ImGui::GetWindowPos().y;

    ImVec2 ButtonPos1 = {ImGui::GetWindowSize().x * 0.485f, ImGui::GetWindowSize().y * 0.3f};
    ImVec2 ButtonPos2 = {ImGui::GetWindowSize().x * 0.515f, ImGui::GetWindowSize().y * 0.3f};
    ImGui::SetCursorPos(ButtonPos1);
    float lineHeight = ImGui::GetIO().FontDefault->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight , lineHeight };

    const float original = Editor::m_font_awesome->Scale;
    Editor::m_font_awesome->Scale = 0.5f;
    ImGui::PushFont( Editor::m_font_awesome );

    if (ImGui::Button(ICON_FA_PLAY, buttonSize))
      GetEnv().pECS->GetWorld().SetPause(false);
    ImGui::SetCursorPos(ButtonPos2);
    if (ImGui::Button(ICON_FA_PAUSE, buttonSize))
      GetEnv().pECS->GetWorld().SetPause(true);

    ImGui::PopFont();
    Editor::m_font_awesome->Scale = original;

    ImGui::End();
  }
}
