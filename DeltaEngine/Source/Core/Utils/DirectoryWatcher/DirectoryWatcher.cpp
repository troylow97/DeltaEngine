#include "DirectoryWatcher.h"

#include "DirectoryListener.h"
#include "Core/Utils/FileUtils.h"
#include "Core/Debugging/Logger/Log.h"
namespace DeltaEngine
{

void Directory::Initialize()
{
  file_vec.reserve( 256 );
  sub_dir.reserve( 32 );
  for ( auto &file : FileUtils::FileList( cur_dir ) )
    file_vec.push_back( file );

  for ( auto &dir : FileUtils::DirList( cur_dir ) )
  {
    auto str = dir.path().generic_string();
    DeltaEngine_CORE_TRACE( "Sub Directory - {}", str.substr( str.find( FileUtils::Root().filename().generic_string() ) )); 
    sub_dir.emplace_back( Directory { dir } ).Initialize();
  }
}

void SystemDirectory::Initialize()
{
  DeltaEngine_CORE_INFO( "Initializing System Directories" );
  for ( auto &dir : FileUtils::DirList() )
  {
    auto str = dir.path().generic_string();
    DeltaEngine_CORE_TRACE( "Main Directory - {}", str.substr( str.find( FileUtils::Root().filename().generic_string() ) )); 
    m_directories.emplace_back( Directory { dir } ).Initialize();
  }

  m_watcher = new FileWatcher( FileUtils::Root() );
}

void SystemDirectory::AddListener( IFileWatcherListener *listener )
{
  m_watcher->AddListener( listener );
}

void SystemDirectory::StartWatch()
{
  DeltaEngine_CORE_INFO( "Start DirectoryListener" );
  m_watcher->AddListener( new DirectoryListener() );
  m_watcher->Start();
}

void SystemDirectory::StopWatch()
{
  DeltaEngine_CORE_INFO( "Stop DirectoryListener" );
  m_watcher->Stop();
}

std::vector<Directory>& SystemDirectory::Directories()
{
  return m_directories;
}

const std::vector<Directory>& SystemDirectory::ConstDirectories()
{
  return m_directories;
}

void SystemDirectory::Shutdown()
{
  DeltaEngine_CORE_INFO( "Shutting down System Directories..." );
  delete m_watcher;
  DeltaEngine_CORE_INFO( "Shutting down System Directories successful" );
}

}