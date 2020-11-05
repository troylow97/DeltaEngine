#pragma once
#include <filesystem>

namespace DeltaEngine
{
  struct IFileWatcherListener
  {
    virtual void OnFileAdded(std::filesystem::path path) = 0;
    virtual void OnFileDeleted(std::filesystem::path path) = 0;
    virtual void OnFileChanged(std::filesystem::path path) = 0;
    virtual void OnFileRenamed(std::filesystem::path path) = 0;
    virtual ~IFileWatcherListener() = default;
  };
} // namespace DeltaEngine
