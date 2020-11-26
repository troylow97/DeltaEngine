#pragma once
#include <filesystem>

namespace DeltaEngine
{
  struct IFileWatcherListener
  {
    virtual void OnFileAdded(const std::filesystem::path path) = 0;
    virtual void OnFileDeleted(const std::filesystem::path path) = 0;
    virtual void OnFileChanged(const std::filesystem::path path) = 0;
    virtual void OnFileRenamed(const std::filesystem::path path) = 0;
    virtual ~IFileWatcherListener() = default;
  };
} // namespace DeltaEngine
