#include "ImGui/Panels/TilemapPanel.h"
#include "Input/InputManager.h"

#include "Core/GlobalStruct.h"
#include "Render/Texture.h"
#include "Assets/AssetManager.h"
#include "Core/Utils/FileUtils.h"
#include "ECS/ECSModule.h"
#include "ImGui/DropManager.h"
#include "ImGui/Editor.h"

#include "Core/Utils/DirectoryWatcher/DirectoryWatcher.h"

namespace DeltaEngine
{
  std::string selected_tile;
  const Directory* tile_dir;

  void Recurse(const Directory& dir)
  {
    for (const auto& d : dir.sub_dir)
    {
      if (d.cur_dir.path().generic_string().find("Tilemap") != std::string::npos)
        tile_dir = &d;
      else
        Recurse(d);
    }
  }

  TilemapPanel::TilemapPanel(std::string str) :
    IPanel(str)
  {
    m_enabled = true;
    for (const auto& dir : SystemDirectory::Instance().ConstDirectories())
    {
      if (dir.cur_dir.path().generic_string().find("Tilemap") != std::string::npos)
        tile_dir = &dir;
      else
        Recurse(dir);
    }
  }

  TilemapPanel::~TilemapPanel()
  {
    m_enabled = false;
  }

  void TilemapPanel::Render()
  {
    if (ImGui::Begin(m_name.c_str(), &m_enabled/*, ImGuiWindowFlags_HorizontalScrollbar*/))
    {
      if (ImGui::BeginChild("Tile Selection"))
      {
        ImGui::Text("Tileset:");


        float nWidth = ImGui::GetWindowContentRegionWidth();
        int columns = nWidth / 74;
        columns = columns < 1 ? 1 : columns;
        ImGui::Columns(columns, nullptr, false);
        ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.30f, 0.10f, 0.14f, 0.00f ) );

        if (!SystemDirectory::Instance().m_lock.load())
        {
          for (const auto& file : tile_dir->file_vec)
          {
            if (file.extension() == ".png")
            {
              auto str = file.generic_string();
              str = str.substr(str.find(FileUtils::Root().filename().generic_string()));
              str = str.substr(str.find_first_of('/') + 1);
              auto key = str.substr(0,
                                    str.find_last_of('.'));
              auto ref = env.pManager->Get<Texture2D>(key);

              for (size_t i = 0; i < ref->textureInfo.size(); i++)
              {
                Sprite sprite = {ref->GetName(), static_cast<unsigned>(i)};
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
                  ImGui::Image(reinterpret_cast<void*>(id),
                               ImVec2{64, 64},
                               ImVec2{sprite.GetOffset().x, sprite.GetOffset().y},
                               ImVec2{
                                 sprite.GetOffset().x + sprite.GetTiling().x,
                                 sprite.GetOffset().y + sprite.GetTiling().y
                               });
                  ImGui::Text(selected_tile.c_str());
                  ImGui::EndDragDropSource();
                }
              }
            }
          }
        }

        ImGui::PopStyleColor();
      }
      ImGui::EndChild();

      if (ImGui::BeginDragDropTarget())
      {
        const ImGuiPayload* assetpayload = ImGui::AcceptDragDropPayload("ASSETFILES");
        if (assetpayload)
        {
          std::string assetpayload_n = *static_cast<std::string*>(assetpayload->Data);
          std::filesystem::path file{assetpayload_n};
          FileUtils::CopyFileW(file, tile_dir->cur_dir.path() / file.filename());
        }
        else if (ImGui::AcceptDragDropPayload("Explorer Files"))
        {
          for (const auto& path : DropManager::drop_vec)
          {
            auto dest = tile_dir->cur_dir.path() / path.filename();
            FileUtils::CopyFileW(path, dest);
          }
          DropManager::drop_vec.clear();
          Editor::drag = false;
        }


        ImGui::EndDragDropTarget();
      }

    }
    ImGui::End();
  }
}
