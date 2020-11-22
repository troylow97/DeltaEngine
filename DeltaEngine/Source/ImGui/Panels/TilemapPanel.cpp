#include "ImGui/Panels/TilemapPanel.h"
#include "Input/InputManager.h"

#include "Core/GlobalStruct.h"
#include "Render/Texture.h"
#include "Assets/AssetManager.h"
#include "ECS/ECSModule.h"
#include "Core/Utils/FileUtils.h"

namespace DeltaEngine
{
  std::string selected_tile;

  TilemapPanel::TilemapPanel(std::string str) :
    IPanel(str)
  {
    m_enabled = true;
  }

  TilemapPanel::~TilemapPanel()
  {
    m_enabled = false;
  }

  bool TilemapPanel::DraggedFileIn()
  {
    if (InputManager::Instance().CurrentPosition().point_x >= GetTopLeft().x && InputManager::Instance().
      CurrentPosition().point_x <= GetBottomRight().x
      && InputManager::Instance().CurrentPosition().point_y >= GetTopLeft().y && InputManager::Instance().
      CurrentPosition().point_y <= GetBottomRight().y)
    {
      std::cout << "it is in tileset panel!!!" << std::endl;
      return true;
    }
    return false;
  }

  void TilemapPanel::Render(bool isdragged)
  {
    ImGui::Begin(m_name.c_str(), &m_enabled/*, ImGuiWindowFlags_HorizontalScrollbar*/);

    topLeft = ImGui::GetWindowContentRegionMin();
    bottomRight = ImGui::GetWindowContentRegionMax();

    topLeft.x += ImGui::GetWindowPos().x;
    topLeft.y += ImGui::GetWindowPos().y;
    bottomRight.x += ImGui::GetWindowPos().x;
    bottomRight.y += ImGui::GetWindowPos().y;

    ImGui::Text("Tileset:");

    if (isdragged)
    {
      DraggedFileIn();
    }

    float nWidth = ImGui::GetWindowContentRegionWidth();
    int columns = nWidth / 74;
    columns = columns < 1 ? 1 : columns;
    ImGui::Columns( columns, nullptr, false );

    for (const auto& file : FileUtils::FileList("Tilemap"))
    {
      DeltaEngine_CORE_TRACE( "File - {}", file.generic_string() );
      if (file.extension() == ".png")
      {
        auto key = file.generic_string().substr(0,
                                                file.generic_string().find_last_of('.'));
        auto ref = env.pManager->Get<Texture2D>(key);

        for (size_t i = 0; i < ref->textureInfo.size() - 1; i ++)
        {
          Sprite sprite = { ref->GetName(), static_cast<unsigned>(i) };
          uint64_t id = sprite.GetTexture()->GetRendererID();

          ImGui::PushID(static_cast<int>(i));

          if (ImGui::ImageButton(reinterpret_cast<void*>(id),
               ImVec2{64, 64},
               ImVec2{sprite.GetOffset().x, sprite.GetOffset().y},
               ImVec2{
                 sprite.GetOffset().x + sprite.GetTiling().x,
                 sprite.GetOffset().y + sprite.GetTiling().y
               }))
          {
            // Nothing for now
          }

          //float last_tile_x2 = ImGui::GetItemRectMax().x;
          //float next_tile_x2 = last_tile_x2 + style.ItemSpacing.x + 32.0f;
          //// Expected position if next tile was on same line
          //if (next_tile_x2 < window_visible_x2)
          //  ImGui::SameLine();
          ImGui::PopID();
          ImGui::NextColumn();

          ImGuiDragDropFlags src_flags = 0;
          src_flags |= ImGuiDragDropFlags_SourceNoDisableHover; // Keep the source displayed as hovered
          src_flags |= ImGuiDragDropFlags_SourceAllowNullID;
          // Allow items such as Text(), Image() that have no unique identifier to be used as drag source, by manufacturing a temporary identifier based on their window-relative position. This is extremely unusual within the dear imgui ecosystem and so we made it explicit

          if (ImGui::BeginDragDropSource(src_flags))
          {
            selected_tile.assign(key + '_' + std::to_string(sprite.m_Index));
            ImGui::SetDragDropPayload("TILES", &selected_tile, sizeof(std::string));
            InputManager::Instance().SetTilesetDragged(true);
            ImGui::Image(reinterpret_cast<void*>(id),
                          ImVec2{64, 64},
                          ImVec2{sprite.GetOffset().x, sprite.GetOffset().y},
                          ImVec2{
                            sprite.GetOffset().x + sprite.GetTiling().x, sprite.GetOffset().y + sprite.GetTiling().y
                          });
            ImGui::Text(selected_tile.c_str());
            ImGui::EndDragDropSource();
          }
        }
      }
    }

    if (ImGui::BeginDragDropTarget())
    {
      ImGuiDragDropFlags target_flags = 0;

      const ImGuiPayload* assetpayload = ImGui::AcceptDragDropPayload("ASSETFILES", target_flags);
      if (assetpayload)
      {
        std::string assetpayload_n = *static_cast<std::string*>(assetpayload->Data);
        std::wstring assetpayload_nws(assetpayload_n.begin(), assetpayload_n.end());
        std::size_t index = assetpayload_nws.find_last_of(L"/\\");
        std::wstring newFileName;
        std::wstring newPathName = L"Tilemap/";
        for (size_t i = index; i < assetpayload_nws.length(); ++i)
        {
          newFileName += assetpayload_nws[i];
        }
        newPathName += newFileName;

        FileUtils::CopyFileW(assetpayload_nws, newPathName);
      }
      ImGui::EndDragDropTarget();
    }
    ImGui::End();
  }
}
