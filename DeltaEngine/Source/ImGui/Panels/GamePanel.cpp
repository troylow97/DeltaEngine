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
#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"

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
      if (!Camera::allCameras.empty() )
      {
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        render_pos = {ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y};
        render_size = {viewportPanelSize.x, viewportPanelSize.y};
        if (ImGui::IsWindowHovered())
        {
            ImGui::SetWindowFocus();
            //mouse_pos = { ImGui::GetMousePos().x, ImGui::GetMousePos().y };

            prev_mouse = curr_mouse;
 
            auto& t = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>({ 0 });
            float cameraWidth = Camera::allCameras[0]->Max(t).x - Camera::allCameras[0]->Min(t).x;
            float cameraHeight = Camera::allCameras[0]->Max(t).y - Camera::allCameras[0]->Min(t).y;
            
            float cursorViewPortDistanceX = ImGui::GetMousePos().x - render_pos.x;
            float cursorViewPortDistanceY = ImGui::GetMousePos().y - render_pos.y;
            curr_mouse.point_x = ((cursorViewPortDistanceX / render_size.x) * cameraWidth) + Camera::allCameras[0]->Min(Camera::editorCameraTransform).x;
            curr_mouse.point_y = Camera::allCameras[0]->Max(Camera::editorCameraTransform).y - ((cursorViewPortDistanceY / render_size.y) * cameraHeight);
        }
        Camera::allCameras[0]->SetAspectRatio(viewportPanelSize.x, viewportPanelSize.y);
        Camera::allCameras[0]->SetViewportSize(viewportPanelSize.x);
        uint64_t textureID = Camera::allCameras[0]->GetFrameBuffer().GetColorAttachment();
        ImGui::Image(reinterpret_cast<void*>(textureID), viewportPanelSize, ImVec2{0, 1}, ImVec2{1, 0});
      }
    }
    ImGui::End();
  }
}
