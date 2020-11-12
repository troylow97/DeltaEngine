#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include "Core/Utils/FileUtils.h"
#include "Core/Utils/FileWatcher.h"

namespace DeltaEngine
{

namespace fs = std::filesystem;

class EditorDirectoryWatcher
{

  std::unordered_map<fs::directory_entry, std::vector<std::string>> m_directory_map;
  void Initialize();
  void Shutdown();
  static EditorDirectoryWatcher *Get();

};




}



