#include "SpriteEditorPanel.h"
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

  void SpriteEditorPanel::Render(bool)
  {
    ImGui::Begin(m_name.c_str(), &m_enabled, ImGuiWindowFlags_MenuBar);

    topLeft = ImGui::GetWindowContentRegionMin();
    bottomRight = ImGui::GetWindowContentRegionMax();
    topLeft.x += ImGui::GetWindowPos().x;
    topLeft.y += ImGui::GetWindowPos().y;
    bottomRight.x += ImGui::GetWindowPos().x;
    bottomRight.y += ImGui::GetWindowPos().y;

    ImGui::InputText("Texture Name", textureName, 128);
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

    Texture2D* texture = GetEnv().pManager->Get<Texture2D>(std::string(textureName));
    if (ImGui::BeginMenuBar())
    {
      if (ImGui::BeginMenu("Auto Slicing"))
      {
        static const char* components[]{"Automatic", "Row by Column", "Nothing"};
        static int selected = 0;
        ImGui::Combo("Slice Type", &selected, components, IM_ARRAYSIZE(components));
        if (selected == 0)
        {
          static float pivot[2] = { 0.5f, 0.5f };
          static bool overlap = false;
          ImGui::Checkbox("Allow Overlapping Sprites", &overlap);
          ImGui::DragFloat2("Pivot", pivot, 0.01f, 0.0f, 1.0f);
          if (ImGui::Button("Auto Slice"))
          {
            if (texture)
              // Slice it
              texture->AutoSlice(Vector2(pivot[0], pivot[1]), overlap);
          }
        }
        if (selected == 1)
        {
          static int cr[2] = { 1, 1 };
          static float pivot[2] = { 0.5f, 0.5f };
          ImGui::DragInt2("Row and Column", cr, 0.25f, 1, 128);
          ImGui::DragFloat2("Pivot", pivot, 0.01f, 0.0f, 1.0f);
          if (ImGui::Button("Auto Slice"))
          {
            if (texture)
              // Slice it
              texture->SliceAll(cr[0], cr[1], Vector2(pivot[0], pivot[1]));
          }
        }
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }

    if (texture)
    {
      uint64_t textureID = texture->GetRendererID();
      static float zoom = 1.0f;

      ImVec2 p = ImGui::GetCursorScreenPos();
      ImGui::BeginChild("Texture Editing");
      ImGui::Image(
        reinterpret_cast<void*>(textureID),
        ImVec2{ texture->GetWidth() * zoom, texture->GetHeight() * zoom },
        ImVec2{ 0, 0 }, ImVec2{ 1, 1 }, ImVec4{ 1, 1, 1, 1 }, ImVec4{ 1, 1, 1, 1 });
      int i = 0;
      for (const TextureInfo& info : texture->textureInfo)
      {
        ImGui::PushID((texture->GetName() + std::to_string(i++)).c_str());
        ImGui::GetWindowDrawList()->AddRectFilled(
          ImVec2{ p.x + info.offset.x + 1, p.y + info.offset.y + 1 },
          ImVec2{ p.x + info.offset.x + info.size.x + 1, p.y + info.offset.y + info.size.y + 1 },
          IM_COL32(0, 255, 0, 51));
        ImGui::GetWindowDrawList()->AddRect(
          ImVec2{ p.x + info.offset.x + 1, p.y + info.offset.y + 1 },
          ImVec2{ p.x + info.offset.x + info.size.x + 1, p.y + info.offset.y + info.size.y + 1 },
          IM_COL32(0, 255, 0, 51));
        ImGui::GetWindowDrawList()->AddCircle(
          ImVec2{
            p.x + info.offset.x + info.pivot.x * info.size.x + 1,
            p.y + info.offset.y + info.pivot.y * info.size.y + 1 },
          5.0f, IM_COL32(0, 255, 0, 128), 0, 2.0f);
        ImGui::PopID();
      }
      ImGui::EndChild();
    }
    ImGui::End();
  }
}
