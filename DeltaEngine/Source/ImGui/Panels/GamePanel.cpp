/**********************************************************************************
* \file   GamePanel.cpp
* \brief  The file contains implementation of GamePanel.
* \author Ong, Graeme,   100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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

      for (size_t i = 0; i < Camera::allCameras.size(); ++i)
      {
        Camera::allCameras[i]->SetAspectRatio(viewportPanelSize.x, viewportPanelSize.y);
        Camera::allCameras[i]->SetViewportSize(viewportPanelSize.x);
      }
      uint64_t textureID = Camera::finalFrameBuffer->GetColorAttachment();
      ImGui::Image(reinterpret_cast<void*>(textureID), viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    }
    ImGui::End();
  }
}
