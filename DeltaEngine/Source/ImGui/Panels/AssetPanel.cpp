#include "AssetPanel.h"
#include "Input/InputManager.h"
#include "ImGui/IconsFontAwesome5.h"

#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"

#include "Core/Utils/FileUtils.h"
#include "Core/Utils/FileDialog.h"
#include "Core/Utils/IFileWatcherListener.h"
#include "Assets/AssetManager.h"

namespace DeltaEngine
{

std::filesystem::path selection;
std::string selection_file;

void RecursiveDirectoryNodes( std::filesystem::directory_entry dir )
{
    static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_None;

    ImGuiTreeNodeFlags node_flags = base_flags;
    node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet

    ImGui::TreeNodeEx(dir.path().filename().generic_string().c_str(), node_flags);
    if (ImGui::IsItemClicked())
    {
        selection = dir;

        for (auto& ref : FileUtils::DirList(dir))
        {
            if (dir.path().stem().has_extension() == 0)
            {
                ImGui::SetNextTreeNodeOpen(true);
                RecursiveDirectoryNodes(ref);
            }
        }
    }
}

AssetPanel::AssetPanel( std::string str ) :
  IPanel( str )
{
  m_enabled = true;
}
AssetPanel::~AssetPanel()
{
  m_enabled = false;
}

bool AssetPanel::DraggedFileIn()
{
  if ( InputManager::Get()->CurrentPosition().point_x >= GetTopLeft().x && InputManager::Get()->CurrentPosition().point_x <= GetBottomRight().x
       && InputManager::Get()->CurrentPosition().point_y >= GetTopLeft().y && InputManager::Get()->CurrentPosition().point_y <= GetBottomRight().y )
  {
    std::cout << "it is in Asset panel!!!" << std::endl;
    return true;
  }
  return false;
}


void AssetPanel::Render( bool isdragged )
{
  ImGui::Begin( m_name.c_str(), &m_enabled );
  float width = ImGui::GetContentRegionAvailWidth();
  float height = ImGui::GetContentRegionAvail().y;
  ImGui::BeginChild( "Directories", { width/3, height }, true );
  if ( ImGui::TreeNode( "Folders" ) )
  {
    for ( auto ref : FileUtils::DirList() )
      RecursiveDirectoryNodes( ref );
    ImGui::TreePop();
  }
  ImGui::EndChild();
  ImGui::SameLine();
  float nWidth = ImGui::GetContentRegionAvailWidth();
  ImGui::SameLine();
  ImGui::BeginChild( "Details", { nWidth, height }, true );
  if (!selection.empty())
  {
      static ImGuiTextFilter filter;
      ImGui::Text("Filter usage:\n"
          "  \"\"         display all lines\n"
          "  \"abc\"      display lines containing \"abc\"\n"
          "  \"abc,123\"  display lines containing \"abc\" or \"123\"\n"
          "  \"-abc\"     hide lines containing \"abc\"");
      ImGui::Text("");
      filter.Draw();
      ImGui::Text("");

      for (auto ref : FileUtils::FileList(selection))
      {
          if (filter.PassFilter(ref.filename().generic_string().c_str()))
          {
              if (ref.extension() == ".anim" || ref.extension() == ".clip")
              {
                  ImGui::Button(ICON_FA_PHOTO_VIDEO, { 22.0f, 18.0f });
                  ImGui::SameLine();
                  ImGui::Text(ref.filename().generic_string().c_str());
              }
              else if (ref.extension() == ".wav")
              {
                  ImGui::Button(ICON_FA_MUSIC, { 22.0f, 18.0f });
                  ImGui::SameLine();
                  ImGui::Text(ref.filename().generic_string().c_str());
              }
              else if (ref.extension() == ".ttf")
              {
                  ImGui::Button(ICON_FA_FONT, { 22.0f, 18.0f });
                  ImGui::SameLine();
                  ImGui::Text(ref.filename().generic_string().c_str());
              }
              else if (ref.extension() == ".ini")
              {
                  ImGui::Button(ICON_FA_FOLDER_MINUS, { 22.0f, 18.0f });
                  ImGui::SameLine();
                  ImGui::Text(ref.filename().generic_string().c_str());
              }
              else if (ref.extension() == ".fs" || ref.extension() == ".vs" || ref.extension() == ".dat")
              {
                  ImGui::Button(ICON_FA_FILE, { 22.0f, 18.0f });
                  ImGui::SameLine();
                  ImGui::Text(ref.filename().generic_string().c_str());
              }
              else if (ref.extension() == ".json")
              {
                  ImGui::Button(ICON_FA_FILE_CODE, { 22.0f, 18.0f });
                  ImGui::SameLine();
                  ImGui::Text(ref.filename().generic_string().c_str());
              }
              else if (ref.extension() == ".png" || ref.extension() == ".jpg")
              {
                  //uint64_t textureID;
                  //Sprite _sprite = { ref.filename().generic_string().c_str(), 0 };
                  //textureID = _sprite.GetTexture()->GetRendererID();
                  //ImGui::ImageButton(reinterpret_cast<void*>(textureID),
                  //    ImVec2{ 32,32 },
                  //    ImVec2{ _sprite.GetOffset().x, _sprite.GetOffset().y },
                  //    ImVec2{ _sprite.GetOffset().x + _sprite.GetTiling().x, _sprite.GetOffset().y + _sprite.GetTiling().y });
                  ImGui::Button(ICON_FA_FILE_IMAGE, { 22.0f, 18.0f });
                  ImGui::SameLine();
                  ImGui::Text(ref.filename().generic_string().c_str());
              }
              else if (ref.extension() == ".info")
              {
                  ImGui::Button(ICON_FA_STICKY_NOTE, { 22.0f, 18.0f });
                  ImGui::SameLine();
                  ImGui::Text(ref.filename().generic_string().c_str());
              }

              ImGuiDragDropFlags src_flags = 0;
              src_flags |= ImGuiDragDropFlags_SourceNoDisableHover; // Keep the source displayed as hovered
              src_flags |= ImGuiDragDropFlags_SourceAllowNullID;    // Allow items such as Text(), Image() that have no unique identifier to be used as drag source, by manufacturing a temporary identifier based on their window-relative position. This is extremely unusual within the dear imgui ecosystem and so we made it explicit

              if (ImGui::BeginDragDropSource(src_flags))
              {
                selection_file.assign( ref.generic_string().c_str());
                ImGui::SetDragDropPayload("ASSETFILES", &selection_file, sizeof(std::string));
                ImGui::Text(ref.filename().generic_string().c_str());
                ImGui::EndDragDropSource();
              }
              /*
                if (ImGui::BeginDragDropTarget())
                {
                    ImGuiDragDropFlags target_flags = 0;

                    const ImGuiPayload* assetpayload = ImGui::AcceptDragDropPayload("ASSETFILES", target_flags);
                    if (assetpayload)
                    {
                        std::string assetpayload_n = *(std::string*)(assetpayload->Data);
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
              */
          }
      }
  }
  ImGui::EndChild();

  ImGui::End();
}
}
