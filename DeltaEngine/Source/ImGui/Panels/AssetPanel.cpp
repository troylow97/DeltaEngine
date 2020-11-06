#include "AssetPanel.h"
#include "Input/InputManager.h"

#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"

#include "Core/Utils/FileUtils.h"
#include "Core/Utils/FileDialog.h"
#include "Core/Utils/IFileWatcherListener.h"
#include "Assets/AssetManager.h"

namespace DeltaEngine
{

struct AssetDirectoryListener : public IFileWatcherListener
{
  void OnFileAdded(std::filesystem::path path) override
  {
    //if(path.extension() == ".png" || path.extension() == ".jpg")

    //GetEnv()->pManager->Get<>()
  }
  void OnFileChanged(std::filesystem::path path) override;
  void OnFileDeleted(std::filesystem::path path) override;
  void OnFileRenamed(std::filesystem::path path) override;
};



std::filesystem::path selection;

void RecursiveDirectoryNodes( std::filesystem::directory_entry dir )
{
  bool flag = false;
  for ( auto &ref : FileUtils::DirList( dir ) )
    if ( ImGui::TreeNode( dir.path().filename().generic_string().c_str() ) )
    {
      flag = true;
      RecursiveDirectoryNodes( ref );
      ImGui::TreePop();
    }

  if (!flag )
  {
    static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_None;

    ImGuiTreeNodeFlags node_flags = base_flags;
    node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet

    ImGui::TreeNodeEx( dir.path().filename().generic_string().c_str(), node_flags );
    if ( ImGui::IsItemClicked )
    {
      selection = dir;
      DeltaEngine_CORE_TRACE( "Folder {} clicked", dir.path().filename().generic_string() );
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
  if ( !selection.empty() )
    for ( auto ref : FileUtils::FileList( selection ) )
      DeltaEngine_CORE_TRACE( "Files: {}", ref.filename().generic_string() );
  ImGui::EndChild();

  ImGui::End();
}

ImVec2 AssetPanel::GetTopLeft()
{
  return topLeft;
}

ImVec2 AssetPanel::GetBottomRight()
{
  return bottomRight;
}
}
