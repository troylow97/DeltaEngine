#include "AssetPanel.h"
#include "Input/InputManager.h"
#include "ImGui/IconsFontAwesome5.h"

#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"

#include "ImGui/Editor.h"

#include "Core/Utils/FileUtils.h"
#include "Assets/AssetManager.h"

namespace DeltaEngine
{
std::filesystem::path selection;
std::string selection_file;

void RecursiveDirectoryNodes( std::filesystem::directory_entry dir, ImGuiTreeNodeFlags flags )
{

  for ( auto d : FileUtils::DirList(dir) )
  {
    auto vec = FileUtils::DirList(d);
    if (vec.empty() )
      flags = ImGuiTreeNodeFlags_Leaf;

    bool open = ImGui::TreeNodeEx( d.path().filename().generic_string().c_str(), flags );
    if ( ImGui::IsItemClicked() )
      selection = d;

    if (open)
    {
      RecursiveDirectoryNodes( d, flags );
      ImGui::TreePop();
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
  if ( InputManager::Instance().CurrentPosition().point_x >= GetTopLeft().x && InputManager::Instance().
       CurrentPosition().point_x <= GetBottomRight().x
       && InputManager::Instance().CurrentPosition().point_y >= GetTopLeft().y && InputManager::Instance().
       CurrentPosition().point_y <= GetBottomRight().y )
  {
    std::cout << "it is in Asset panel!!!" << std::endl;
    return true;
  }
  return false;
}


void AssetPanel::Render( bool isdragged )
{
  ImGui::Begin( m_name.c_str(), &m_enabled );
  float width = ImGui::GetWindowContentRegionWidth();
  float height = ImGui::GetContentRegionAvail().y;
  ImGui::BeginChild( "Directories", { width / 4, height }, true, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_HorizontalScrollbar  );
  if ( ImGui::TreeNode( "Folders" ) )
  {
    for ( auto dir : FileUtils::DirList() )
    {
      ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
      auto vec = FileUtils::DirList(dir);
      if (vec.empty() )
        flags = ImGuiTreeNodeFlags_Leaf;

      bool open = ImGui::TreeNodeEx( dir.path().filename().generic_string().c_str(), flags );
      if ( ImGui::IsItemClicked() )
        selection = dir;

      if (open)
      {
        RecursiveDirectoryNodes( dir, flags );
        ImGui::TreePop();
      }
    }
    ImGui::TreePop();
  }
  ImGui::EndChild();
  ImGui::SameLine();
  ImGui::SameLine();
  ImGui::BeginChild( "Details", { 0, 0 }, true, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_HorizontalScrollbar  );

  if ( !selection.empty() )
  {
    static ImGuiTextFilter filter;
    ImGui::Text( "Filter usage:\n"
                 "  \"\"         display all lines\n"
                 "  \"abc\"      display lines containing \"abc\"\n"
                 "  \"abc,123\"  display lines containing \"abc\" or \"123\"\n"
                 "  \"-abc\"     hide lines containing \"abc\"" );
    ImGui::Text( "" );
    filter.Draw();
    ImGui::Text( "" );

    int columns = (width - width / 4) / 128;
    columns = columns < 1 ? 1 : columns;
    ImGui::Columns(columns, nullptr, false);
    for ( auto ref : FileUtils::FileList( selection ) )
    {
      if ( filter.PassFilter( ref.filename().generic_string().c_str() ) )
      {
        auto str = ref.generic_string();
        str = str.substr( str.find( FileUtils::Root().filename().generic_string() ) );
        str = str.substr( str.find_first_of( '/' ) + 1 );

        ImGui::PushID( ref.generic_string().c_str() );
        ImGui::BeginGroup();

        const float original = Editor::m_font_awesome->Scale;
        Editor::m_font_awesome->Scale = 2.0f;
        ImGui::PushFont( Editor::m_font_awesome );


        if ( ref.extension() == ".anim" || ref.extension() == ".clip" )
          ImGui::Button( ICON_FA_PHOTO_VIDEO, { 128.0f, 128.0f } );
        else if ( ref.extension() == ".wav" )
          ImGui::Button( ICON_FA_MUSIC, { 128.0f, 128.0f } );
        else if ( ref.extension() == ".ttf" )
          ImGui::Button( ICON_FA_FONT, { 128.0f, 128.0f } );
        else if ( ref.extension() == ".ini" )
          ImGui::Button( ICON_FA_FOLDER_MINUS, { 128.0f, 128.0f } );
        else if ( ref.extension() == ".fs" || ref.extension() == ".vs" || ref.extension() == ".dat" )
          ImGui::Button( ICON_FA_FILE, { 128.0f, 128.0f } );
        else if ( ref.extension() == ".json" )
          ImGui::Button( ICON_FA_FILE_CODE, { 128.0f, 128.0f } );
        else if ( ref.extension() == ".png" || ref.extension() == ".jpg" )
        {
          uint64_t textureID = GetEnv().pManager->Get<Texture2D>(str.substr(0, str.find_last_of('.')))->GetRendererID();
          ImGui::ImageButton(reinterpret_cast<void*>(textureID),
              ImVec2{ 128.0f,128.0f }, {0,0}, {1,1}, 0);
        }
        else if ( ref.extension() == ".info" )
          ImGui::Button( ICON_FA_STICKY_NOTE, { 128.0f, 128.0f } );

        ImGui::PopFont();
        Editor::m_font_awesome->Scale = original;

        ImGui::TextWrapped( ref.filename().generic_string().c_str() );

        ImGui::EndGroup();


        ImGuiDragDropFlags src_flags = 0;
        src_flags |= ImGuiDragDropFlags_SourceNoDisableHover; // Keep the source displayed as hovered
        src_flags |= ImGuiDragDropFlags_SourceAllowNullID;
        // Allow items such as Text(), Image() that have no unique identifier to be used as drag source, by manufacturing a temporary identifier based on their window-relative position. This is extremely unusual within the dear imgui ecosystem and so we made it explicit

        if ( ImGui::BeginDragDropSource( src_flags ) )
        {

          selection_file.assign( str );
          ImGui::SetDragDropPayload( "ASSETFILES", &selection_file, sizeof( std::string ) );
          ImGui::Text( ref.filename().generic_string().c_str() );
          ImGui::EndDragDropSource();
        }
        ImGui::PopID();
        ImGui::NextColumn();
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
