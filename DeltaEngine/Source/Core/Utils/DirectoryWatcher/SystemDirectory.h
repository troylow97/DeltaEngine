/**********************************************************************************
* \file   SystemDirectory.h
* \brief  This file contains the definition for the SystemDirectory
*         The system directory is responsible for managing a cached reference of
*         the mounted directory and the filewatcher
*
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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

    std::vector<Directory>& Directories();
    const std::vector<Directory>& ConstDirectories();
  };
}