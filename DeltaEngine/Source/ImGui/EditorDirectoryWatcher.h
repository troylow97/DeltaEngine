#pragma once
#include <unordered_map>
#include <vector>
#include "Core/Containers/Singleton.h"

namespace DeltaEngine
{
  struct EditorDirectoryWatcher : public Singleton<EditorDirectoryWatcher>
  {
    using Directories = std::unordered_map<std::string, std::vector<std::string>>;
    Directories m_directory_map;
    void Initialize();
    void Shutdown();
  };
}
