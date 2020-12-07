#pragma once

#include "Core/Containers/Singleton.h"
#include "FileWatcher.h"

namespace DeltaEngine
{

struct Directory
{
  std::filesystem::directory_entry cur_dir;
  std::vector<Directory> sub_dir;
  std::vector<std::filesystem::path> file_vec;
  void Initialize();
};

class SystemDirectory : public Singleton<SystemDirectory>
{

  std::vector<Directory> m_directories;
  FileWatcher* m_watcher{nullptr};

public:
  std::atomic<bool> m_lock;
  void Initialize();
  void AddListener(IFileWatcherListener* listener);
  void StartWatch();
  void StopWatch();
  void Shutdown();

  std::vector<Directory> &Directories();
  const std::vector<Directory> &ConstDirectories();
};

}
