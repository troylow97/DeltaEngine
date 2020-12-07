#include "GamePanel.h"

#include "imgui.h"
#include "Render/Camera.h"

namespace DeltaEngine
{
  GamePanel::GamePanel(std::string str, Editor& e)
    : IPanel(str, e)
  {
    m_enabled = true;
  }

  GamePanel::~GamePanel()
  {
    m_enabled = false;
  }

  void GamePanel::Render()
  {
    if (ImGui::Begin(m_name.c_str(), nullptr, ImGuiWindowFlags_MenuBar))
    {
      ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
      render_pos = { ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y };
      render_size = { viewportPanelSize.x,viewportPanelSize.y };

      //camera[0].SetAspectRatio(gameFixedAspectRatio, 1.0f);
      camera[0].SetAspectRatio(viewportPanelSize.x, viewportPanelSize.y);
      camera[0].SetViewportSize(viewportPanelSize.x);
      uint64_t textureID = camera[0].GetFrameBuffer().GetColorAttachment();
      ImGui::Image(reinterpret_cast<void*>(textureID), viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    }
    ImGui::End();
  }
}
