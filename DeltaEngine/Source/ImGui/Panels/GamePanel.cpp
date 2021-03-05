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
      ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
      render_pos = {ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y};
      render_size = {viewportPanelSize.x, viewportPanelSize.y};
      if (ImGui::IsWindowHovered())
      {
        ImGui::SetWindowFocus();

        prev_mouse = curr_mouse;
        
        auto& t = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>({ 0 });
        float cameraWidth = Camera::allCameras[0]->Max(t).x - Camera::allCameras[0]->Min(t).x;  // 12.182
        float cameraHeight = Camera::allCameras[0]->Max(t).y - Camera::allCameras[0]->Min(t).y; //  5.000 
        env.pECS->GetWorld().GetEntityManager().ForEach([&](Transform& t, Camera& c)
        {
          game_camera_mid = { t.position.x ,t.position.y }; // (5, 2)
          game_camera_min = { game_camera_mid.x - (cameraWidth / 2), game_camera_mid.y - (cameraHeight / 2) };  // (-1.091, -0.5)
          game_camera_max = { game_camera_mid.x + (cameraWidth / 2), game_camera_mid.y + (cameraHeight / 2) }; // (11.091,  4.5)
        });

        float cursorViewPortDistanceX = ImGui::GetMousePos().x - render_pos.x;
        float cursorViewPortDistanceY = ImGui::GetMousePos().y - render_pos.y; 
        curr_mouse.point_x = ((cursorViewPortDistanceX / render_size.x) * cameraWidth) + game_camera_min.x;
        curr_mouse.point_y = game_camera_max .y - ((cursorViewPortDistanceY / render_size.y) * cameraHeight);
      }
      //Camera::allCameras[0]->SetAspectRatio(viewportPanelSize.x, viewportPanelSize.y);
      //Camera::allCameras[0]->SetViewportSize(viewportPanelSize.x);
      //uint64_t textureID = Camera::allCameras[0]->GetFrameBuffer().GetColorAttachment();
      //ImGui::Image(reinterpret_cast<void*>(textureID), viewportPanelSize, ImVec2{0, 1}, ImVec2{1, 0});
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
