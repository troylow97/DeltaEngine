/**********************************************************************************
* \file   FileWatcher.h
* \brief  This file contains the definition for filewatcher
*         Filewatcher observes the directory and calls the list of listeners
*         attached
*
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include <thread>
#include <atomic>
#include <vector>
#include <filesystem>
#include <memory>

namespace DeltaEngine
{
  struct IFileWatcherListener;

  class FileWatcher
  {
    std::vector<std::unique_ptr<IFileWatcherListener>> m_listeners;
    std::filesystem::path m_directory;
    std::atomic<bool> m_running{false};
    std::thread m_thread;

  public:
    FileWatcher(std::filesystem::path directory);
    void Directory( std::filesystem::path dir );
    const std::filesystem::path& Directory();
    void AddListener(IFileWatcherListener* listener);
    void Start();
    void Stop();
    void OnFileAdded(std::filesystem::path file);
    void OnFileDeleted(std::filesystem::path file);
    void OnFileChanged(std::filesystem::path file);
    void OnFileRenamed(std::filesystem::path file);

    static void Thread(FileWatcher& fileWatcher);
  };
} // namespace DeltaEngine
