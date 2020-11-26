#include "DirectoryWatcher.h"

#include "DirectoryListener.h"
#include "Core/Utils/FileUtils.h"
namespace DeltaEngine
{

void Directory::Initialize()
{
  file_vec.reserve( 256 );
  sub_dir.reserve( 32 );
  for ( auto &file : FileUtils::FileList( cur_dir ) )
    file_vec.push_back( file );

  for ( auto &dir : FileUtils::DirList( cur_dir ) )
    sub_dir.emplace_back( Directory { dir } ).Initialize();
}

void SystemDirectory::Initialize()
{
  for ( auto &dir : FileUtils::DirList() )
    m_directories.emplace_back( Directory { dir } ).Initialize();

  m_watcher = new FileWatcher( FileUtils::Root() );
}

void SystemDirectory::AddListener( IFileWatcherListener *listener )
{
  m_watcher->AddListener( listener );
}

void SystemDirectory::StartWatch()
{
  m_watcher->AddListener( new DirectoryListener() );
  m_watcher->Start();
}

void SystemDirectory::StopWatch()
{
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
  delete m_watcher;
}

}