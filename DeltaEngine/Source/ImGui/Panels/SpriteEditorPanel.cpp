#include "SpriteEditorPanel.h"

#include "imgui.h"
#include "Core/GlobalStruct.h"
#include "Assets/AssetManager.h"
#include "Render/Texture.h"
#include "Input/InputManager.h"

namespace DeltaEngine
{
  SpriteEditorPanel::SpriteEditorPanel(std::string str)
    : IPanel(str)
  {
    m_enabled = true;
  }

  SpriteEditorPanel::~SpriteEditorPanel()
  {
    m_enabled = false;
  }

  void SpriteEditorPanel::Render()
  {
    ImGui::Begin(m_name.c_str(), &m_enabled, ImGuiWindowFlags_MenuBar);

    ImGui::InputText("Texture Name", textureName, 128);
    Texture2D* texture = GetEnv().pManager->Get<Texture2D>(std::string(textureName));
    if (ImGui::BeginMenuBar())
    {
      if (ImGui::BeginMenu("Auto Slicing"))
      {
        static const char* components[]{"Row by Column"};
        static int selected = 0;
        static int cr[2] = {1, 1};
        //static int offset[2] = { 0.0f, 0.0f };
        //static int padding[2] = { 0.0f, 0.0f };
        //static float pivot[2] = { 0.5f, 0.5f };
        ImGui::Combo("Slice Type", &selected, components, IM_ARRAYSIZE(components));
        if (selected == 0)
        {
          ImGui::DragInt2("Row and Column", cr, 0.25f, 1, 128);
          //ImGui::DragFloat2("Pivot", pivot, 0.01f, 0.0f, 1.0f);
          if (ImGui::Button("Auto Slice"))
          {
            if (texture)
              // Slice it
              texture->SliceAll(cr[0], cr[1]);
          }
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }
    if (texture)
    {
      uint64_t textureID = texture->GetRendererID();

      ImGui::Image(
        reinterpret_cast<void*>(textureID),
        ImVec2{texture->GetWidth() * 0.1f, texture->GetHeight() * 0.1f},
        ImVec2{0, 0}, ImVec2{1, 1}, ImVec4{1, 1, 1, 1}, ImVec4{1, 1, 1, 1});
    }
    if (ImGui::BeginDragDropTarget())
    {
      ImGuiDragDropFlags target_flags = 0;

      const ImGuiPayload* assetpayload = ImGui::AcceptDragDropPayload("ASSETFILES", target_flags);
      if (assetpayload)
      {
        std::string assetpayload_n = *static_cast<std::string*>(assetpayload->Data);

        // find comments after start of argument
        std::size_t pos;
        pos = assetpayload_n.find(".png");
        if (pos != std::string::npos)
          assetpayload_n.erase(pos);
        pos = assetpayload_n.find("Texture");
        assetpayload_n.erase(0, pos);
        strcpy_s(textureName, assetpayload_n.c_str());
      }
      ImGui::EndDragDropTarget();
    }

    ImGui::End();
  }
}
