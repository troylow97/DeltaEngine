/**********************************************************************************
* \file   DirectoryListener.cpp
* \brief  This file contains the implementation for the Directory Listener
*         Directory Listener is responsible for updating the cached directory
*         reference stored in the system directory
*
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "DirectoryListener.h"
#include "SystemDirectory.h"
#include "Core/Utils/FileUtils.h"

namespace DeltaEngine
{

void Recurse( Directory &dir, Directory* selection, const std::filesystem::path& parent )
{
  for ( auto &d : dir.sub_dir )
  {
    if ( d.cur_dir.path().generic_string().find( parent.parent_path().generic_string() ) != std::string::npos )
      selection = &d;
    else
      Recurse( d, selection, parent );
  }
}

  void DirectoryListener::OnFileAdded( std::filesystem::path path )
  {
    SystemDirectory::Instance().m_lock.store( true );
    auto p = FileUtils::Root() / path;
    if ( p.extension() == ".json" )
      return;
    Directory *selection { nullptr };
    for ( auto &dir : SystemDirectory::Instance().Directories() )
      if ( dir.cur_dir.path().generic_string().find( p.parent_path().generic_string() ) != std::string::npos )
        selection = &dir;
      else
        Recurse( dir, selection, p );

    selection->file_vec.push_back(p);
    SystemDirectory::Instance().m_lock.store( false );
  }

  void DirectoryListener::OnFileDeleted( std::filesystem::path path )
  {
    //auto folder_offset = path.find_last_of( '/' );
    //auto folder = path.substr( 0, folder_offset );
    //auto file = path.substr( folder_offset + 1 );

    //auto &filelist = EditorDirectoryWatcher::Instance().m_directory_map[folder];
    //filelist.erase( std::remove_if( filelist.begin(), filelist.end(),
    //                [&]( const std::string &str )
    //{
    //  return str == file;
    //} ) );
  }

  void DirectoryListener::OnFileChanged( std::filesystem::path path )
  {
    (void) path;
  }

  void DirectoryListener::OnFileRenamed( std::filesystem::path path )
  {
    //auto folder_offset = path.find_last_of( '/' );
    //auto folder = path.substr( 0, folder_offset );
    //auto file = path.substr( folder_offset + 1 );

    //auto &filelist = EditorDirectoryWatcher::Instance().m_directory_map[folder];
    //filelist.erase( std::remove_if( filelist.begin(), filelist.end(),
    //                [&]( const std::string &str )
    //{
    //  return str == file;
    //} ) );

    //for ( auto f : FileUtils::FileList( folder ) )
    //  if ( std::find( filelist.begin(), filelist.end(), f ) == filelist.end() )
    //    OnFileAdded( folder + '/' + f.filename().generic_string() );
  }
};