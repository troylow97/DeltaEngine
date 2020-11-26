#pragma once
#include "IFileWatcherListener.h"

namespace DeltaEngine
{
struct DirectoryListener : IFileWatcherListener
{
  void OnFileAdded( std::filesystem::path path ) override;
  void OnFileDeleted( std::filesystem::path path ) override;
  void OnFileChanged( std::filesystem::path path ) override;
  void OnFileRenamed( std::filesystem::path path ) override;
};
}