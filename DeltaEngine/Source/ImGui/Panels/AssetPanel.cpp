#include "AssetPanel.h"
#include "ImGui/IconsFontAwesome5.h"

#include "Core/GlobalStruct.h"

#include "ImGui/Editor.h"

#include "Core/Utils/FileUtils.h"
#include "Core/Utils/DirectoryWatcher/DirectoryWatcher.h"

#include "Assets/AssetManager.h"
#include "ImGui/DropManager.h"

namespace DeltaEngine
{
const Directory *selection { nullptr };
std::string selection_file;

void RecursiveDirectoryNodes( const Directory &dir, ImGuiTreeNodeFlags flags )
{
  for ( const auto &d : dir.sub_dir )
  {
    if ( d.sub_dir.empty() )
      flags = ImGuiTreeNodeFlags_Leaf;

    if ( selection )
      if ( selection->cur_dir == dir.cur_dir )
        flags |= ImGuiTreeNodeFlags_Selected;

    bool open = ImGui::TreeNodeEx( d.cur_dir.path().filename().generic_string().c_str(), flags );
    if ( ImGui::IsItemClicked() )
      selection = &d;

    if ( open )
    {
      RecursiveDirectoryNodes( d, flags );
      ImGui::TreePop();
    }
  }
}

AssetPanel::AssetPanel( std::string str, Editor &e ) :
  IPanel( str, e )
{
  m_enabled = true;
}

AssetPanel::~AssetPanel()
{
  m_enabled = false;
}


void AssetPanel::Render()
{
  m_active = ImGui::Begin( m_name.c_str(), &m_enabled );
  if (m_active)
  {
    float width = ImGui::GetWindowContentRegionWidth();
    float height = ImGui::GetContentRegionAvail().y;
    if ( ImGui::BeginChild( "Directories", { width / 4, height }, true,
         ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_HorizontalScrollbar ) )
      if ( ImGui::CollapsingHeader( "Folders", ImGuiTreeNodeFlags_DefaultOpen ) )
        for ( const auto &dir : SystemDirectory::Instance().ConstDirectories() )
        {
          ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

          if ( dir.sub_dir.empty() )
            flags = ImGuiTreeNodeFlags_Leaf;

          if ( selection )
            if ( selection->cur_dir == dir.cur_dir )
              flags |= ImGuiTreeNodeFlags_Selected;

          bool open = ImGui::TreeNodeEx( dir.cur_dir.path().filename().generic_string().c_str(), flags );
          if ( ImGui::IsItemClicked() )
            selection = &dir;

          if ( open )
          {
            RecursiveDirectoryNodes( dir, flags );
            ImGui::TreePop();
          }
        }

    ImGui::EndChild();
    ImGui::SameLine();

    if ( ImGui::BeginChild( "Details", { 0, 0 }, true, ImGuiWindowFlags_AlwaysAutoResize ) )
    {
      if ( selection )
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

        int columns = static_cast<int>( ( width - width / 4.0f ) / 148.0f );
        columns = columns < 1 ? 1 : columns;
        ImGui::Columns( columns, nullptr, false );

        ImGui::PushStyleColor( ImGuiCol_Button, ImVec4( 0.30f, 0.10f, 0.14f, 0.00f ) );

        if ( !SystemDirectory::Instance().m_lock.load() )
        {
          for ( auto &ref : selection->file_vec )
          {
            if ( ref.extension() == ".info" )
              continue;
            if ( filter.PassFilter( ref.filename().generic_string().c_str() ) )
            {
              auto str = ref.generic_string();
              str = str.substr( str.find( FileUtils::Root().filename().generic_string() ) );
              str = str.substr( str.find_first_of( '/' ) + 1 );

              ImGui::PushID( ref.generic_string().c_str() );
              ImGui::BeginGroup();

              const float original = Editor::font_awesome->Scale;
              Editor::font_awesome->Scale = 2.0f;
              ImGui::PushFont( Editor::font_awesome );


              if ( ref.extension() == ".anim" || ref.extension() == ".clip" )
                ImGui::Button( ICON_FA_PHOTO_VIDEO, { 128.0f, 128.0f } );
              else if ( ref.extension() == ".wav" ||
                        ref.extension() == ".ogg" ||
                        ref.extension() == ".mp3")
                ImGui::Button( ICON_FA_MUSIC, { 128.0f, 128.0f } );
              else if ( ref.extension() == ".bank" )
                ImGui::Button( ICON_FA_BOLD, { 128.0f, 128.0f } );
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
                uint64_t textureID = GetEnv().pManager->Get<Texture2D>( str.substr( 0, str.find_last_of( '.' ) ) )->
                  GetRendererID();
                ImGui::ImageButton( reinterpret_cast<void *>( textureID ),
                                    ImVec2 { 128.0f, 128.0f }, { 0, 0 }, { 1, 1 }, 0 );
              }
              ImGui::PopFont();
              Editor::font_awesome->Scale = original;

              ImGui::TextWrapped( ref.filename().generic_string().c_str() );

              ImGui::EndGroup();

              ImGuiDragDropFlags src_flags = ImGuiDragDropFlags_SourceNoDisableHover |
                ImGuiDragDropFlags_SourceAllowNullID;

              if ( ImGui::BeginDragDropSource( src_flags ) )
              {
                selection_file.assign( str );
                ImGui::SetDragDropPayload( "ASSETFILES", &selection_file, sizeof( std::string ) );
                ImGui::Text( ref.filename().generic_string().c_str() );
                ImGui::EndDragDropSource();
              }

              if ( ref.extension() == ".png" || ref.extension() == ".jpg" )
                if ( ImGui::IsItemClicked() )
                  if ( ImGui::IsMouseDoubleClicked( 0 ) )
                  {
                    m_editor.m_panels[10]->Enable();
                    if ( !m_editor.m_panels[10]->IsEnabled() )
                      m_editor.m_panels[10]->Enable();

                    auto key = str.substr( 0, str.find_last_of( '.' ) );
                    m_editor.selectedFile.assign( key );
                  }

              if ( ref.extension() == ".anim" )
                if ( ImGui::IsItemClicked() )
                  if ( ImGui::IsMouseDoubleClicked( 0 ) )
                  {
                    m_editor.m_panels[11]->Enable();
                    if ( !m_editor.m_panels[11]->IsEnabled() )
                      m_editor.m_panels[11]->Enable();

                    auto key = str.substr( 0, str.find_last_of( '.' ) );
                    m_editor.selectedFile.assign( key );
                  }

              ImGui::PopID();
              ImGui::NextColumn();
            }
          }
        }

        ImGui::PopStyleColor();
      }
      if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup))
        {
          ImGui::OpenPopup("Assets Panel Create New Context Menu");
        }
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
      if (ImGui::BeginPopup("Assets Panel Create New Context Menu"))
      {
        ImGui::Text("Create New...");
        ImGui::Separator();
        if (ImGui::MenuItem("Animation Controller"))
        {
          std::string str = "NewController";
          int i = 0; bool hasSameName = false;
          do
          {
            if (hasSameName)
            {
              hasSameName = false;
              str = "NewController" + std::to_string(i++);
            }
            // using filesystem
            for (auto file : FileUtils::FileList("Animation"))
            {
              // using systemdirectory
                  // Store Directory Reference
              for (auto& d : SystemDirectory::Instance().ConstDirectories())
              {
                if (!strcmp(d.cur_dir.path().filename().generic_string().c_str(), str.c_str()))
                {
                  hasSameName = true;
                  break;
                }
              }
            }
          } while (hasSameName);
          AnimationController::CreateNew("Animation/" + str + ".anim");
        }
        ImGui::EndPopup();
      }
      ImGui::PopStyleVar();
    }
    ImGui::EndChild();


    if ( selection )
      if ( ImGui::BeginDragDropTarget() )
      {
        if ( ImGui::AcceptDragDropPayload( "Explorer Files" ) )
        {
          for ( const auto &path : DropManager::drop_vec )
          {
            auto dest = selection->cur_dir.path() / path.filename();
            FileUtils::CopyFileW( path, dest );
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