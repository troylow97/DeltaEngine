#include "EditorDirectoryWatcher.h"
#include "Core/Utils/FileUtils.h"
#include "Core/Utils/FileWatcher.h"
#include "Core/Utils/IFileWatcherListener.h"
#include "Core/Debugging/Logger/Log.h"
#include "Core/GlobalStruct.h"
#include "Assets/AssetManager.h"

namespace DeltaEngine
{
  struct DirectoryListener : public IFileWatcherListener
  {
    void OnFileAdded(const std::string& path) override
    {
      auto folder_offset = path.find_last_of('/');
      auto folder = path.substr(0, folder_offset);
      auto file = path.substr(folder_offset + 1);
      auto key = path.substr(0, path.find_last_of('.'));
      auto extension = file.substr(file.find_last_of('.'));

      DeltaEngine_CORE_TRACE("folder: {}", folder);
      DeltaEngine_CORE_TRACE("file: {}", file);
      DeltaEngine_CORE_TRACE("key: {}", key);
      DeltaEngine_CORE_TRACE("extension: {}", extension);
      DeltaEngine_CORE_TRACE("path: {}", path);

      if (extension == ".clip" && folder == "Clip")
        EditorDirectoryWatcher::Instance().m_directory_map[folder].push_back(file);
      if (extension == ".anim" && folder == "Animation")
        EditorDirectoryWatcher::Instance().m_directory_map[folder].push_back(file);
      if (extension == ".ttf" && folder == "Fonts")
        EditorDirectoryWatcher::Instance().m_directory_map[folder].push_back(file);

      if (folder == "Shaders")
      {
        if (extension == ".fs" && FileUtils::FileExists(key + ".vs"))
          EditorDirectoryWatcher::Instance().m_directory_map[folder].push_back(file);
        if (extension == ".vs" && FileUtils::FileExists(key + ".fs"))
          EditorDirectoryWatcher::Instance().m_directory_map[folder].push_back(file);
      }
      if ((extension == ".png" || extension == ".jpg") && folder == "Textures")
        EditorDirectoryWatcher::Instance().m_directory_map[folder].push_back(file);
    }

    void OnFileDeleted(const std::string& path) override
    {
      auto folder_offset = path.find_last_of('/');
      auto folder = path.substr(0, folder_offset);
      auto file = path.substr(folder_offset + 1);

      auto& filelist = EditorDirectoryWatcher::Instance().m_directory_map[folder];
      filelist.erase(std::remove_if(filelist.begin(), filelist.end(),
                                    [&](const std::string& str)
                                    {
                                      return str == file;
                                    }));
    }

    void OnFileChanged(const std::string& path) override
    {
      (void)path;
    }

    void OnFileRenamed(const std::string& path) override
    {
      auto folder_offset = path.find_last_of('/');
      auto folder = path.substr(0, folder_offset);
      auto file = path.substr(folder_offset + 1);

      auto& filelist = EditorDirectoryWatcher::Instance().m_directory_map[folder];
      filelist.erase(std::remove_if(filelist.begin(), filelist.end(),
                                    [&](const std::string& str)
                                    {
                                      return str == file;
                                    }));

      for (auto f : FileUtils::FileList(folder))
        if (std::find(filelist.begin(), filelist.end(), f) == filelist.end())
          OnFileAdded(folder + '/' + f.filename().generic_string());
    }
  };


  FileWatcher* directory_watcher;

  void EditorDirectoryWatcher::Initialize()
  {
    DeltaEngine_CORE_INFO("DeltaEngine Directory Watcher Initialized");
    directory_watcher = new FileWatcher(FileUtils::Root());
    DeltaEngine_CORE_INFO("Directory {}", directory_watcher->GetDirectory().generic_string());
    for (auto dir : FileUtils::DirList(directory_watcher->GetDirectory()))
      for (auto file : FileUtils::FileList(dir))
      {
        auto directory = dir.path().generic_string();
        auto root_offset = directory.find(FileUtils::Root().filename().generic_string());
        auto root_directory = directory.substr(root_offset, directory.size() - root_offset);
        auto folder = root_directory.substr(root_directory.find('/') + 1,
                                            root_directory.size() - root_directory.find('/') - 1);

        m_directory_map[folder].push_back(file.filename().generic_string());
      }


    for (auto& [dir, vec] : m_directory_map)
      for (auto& file : vec)
        DeltaEngine_CORE_INFO("Directory - {}, File - {}", dir, file);

    directory_watcher->AddListener(new DirectoryListener());
    directory_watcher->Start();
  }

  void EditorDirectoryWatcher::Shutdown()
  {
    directory_watcher->Stop();
    delete directory_watcher;
  }
}
