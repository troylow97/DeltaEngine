#include "GamePanel.h"
#include "imgui.h"
#include "Render/Camera.h"

namespace DeltaEngine
{
  GamePanel::GamePanel(std::string str)
    : IPanel(str)
  {
    m_enabled = true;
  }

  GamePanel::~GamePanel()
  {
    m_enabled = false;
  }

  void GamePanel::Render()
  {
    ImGui::Begin(m_name.c_str(), &m_enabled, ImGuiWindowFlags_MenuBar);

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

    for (Camera* camera : Camera::allCameras)
    {
      //camera->SetAspectRatio(gameFixedAspectRatio, 1.0f);
      camera->SetAspectRatio(viewportPanelSize.x, viewportPanelSize.y);
      camera->SetViewportSize(viewportPanelSize.x);
      uint64_t textureID = camera->GetFrameBuffer().GetColorAttachment();
      ImGui::Image(reinterpret_cast<void*>(textureID), viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    }
    ImGui::End();

  }
}
