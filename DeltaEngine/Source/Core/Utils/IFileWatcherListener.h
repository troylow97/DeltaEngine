#pragma once
#include <filesystem>

namespace DeltaEngine
{
  struct IFileWatcherListener
  {
    virtual void OnFileAdded(const std::string& path) = 0;
    virtual void OnFileDeleted(const std::string& path) = 0;
    virtual void OnFileChanged(const std::string& path) = 0;
    virtual void OnFileRenamed(const std::string& path) = 0;
    virtual ~IFileWatcherListener() = default;
  };
} // namespace DeltaEngine
