/**********************************************************************************
* \file   SettingsPanel.cpp
* \brief  The file contains implementation of SettingsPanel. Directly edits the
* editor camera.
* \author Ong, Graeme,   100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "SettingsPanel.h"

#include "imgui.h"
#include "Render/Camera.h"

namespace DeltaEngine
{
  SettingsPanel::SettingsPanel(std::string str, Editor& e) :
    IPanel(str, e)
  {
    m_enabled = true;
  }

  SettingsPanel::~SettingsPanel()
  {
    m_enabled = false;
  }

  void SettingsPanel::Render()
  {
    if (ImGui::Begin(m_name.c_str(), &m_enabled))
    {
      if (ImGui::BeginChild("Camera"))
      {
        static float f = 0.0f;
        ImGui::Text("Edit Camera Props"); // Display some text (you can use a format string too)
        ImGui::DragFloat3("pos", (float*)&Camera::editorCameraTransform.position, 0.01f);
        ImGui::DragFloat("size", static_cast<float*>(&Camera::editorCamera->m_Size), 0.01f);
        ImGui::SliderFloat("rot", &f, -180.0f, 180.0f, "%.1f", 1.0f);
        Camera::editorCameraTransform.rotation = Quaternion::AngleAxis(f, Vector3::forward());
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);
      }
      ImGui::EndChild();
    }
    ImGui::End();
  }
}
