#include "FileWatcher.h"
#include "IFileWatcherListener.h"
#include <windows.h>

namespace DeltaEngine
{

  FileWatcher::FileWatcher(std::filesystem::path directory) : m_directory(directory)
  {
  }

  const std::filesystem::path &FileWatcher::GetDirectory()
  {
    return m_directory;
  }

  void FileWatcher::AddListener(IFileWatcherListener *listener)
  {
    m_listeners.push_back(std::unique_ptr<IFileWatcherListener>(listener));
  }

  void FileWatcher::Start()
  {
    if (!m_running.load(std::memory_order_relaxed))
    {
      m_running.store(true, std::memory_order_relaxed);
      m_thread = std::thread(FileWatcher::Thread, std::ref(*this));
      m_thread.detach();
    }
  }

  void FileWatcher::Stop()
  {
    m_running.store(false, std::memory_order_relaxed);
  }

  void FileWatcher::OnFileAdded(std::filesystem::path file)
  {
    for (auto &ref : m_listeners)
      ref->OnFileAdded(file);
  }
  void FileWatcher::OnFileDeleted(std::filesystem::path file)
  {
    for (auto &ref : m_listeners)
      ref->OnFileDeleted(file);
  }
  void FileWatcher::OnFileChanged(std::filesystem::path file)
  {
    for (auto &ref : m_listeners)
      ref->OnFileChanged(file);
  }
  void FileWatcher::OnFileRenamed(std::filesystem::path file)
  {
    for (auto &ref : m_listeners)
      ref->OnFileRenamed(file);
  }

  void FileWatcher::Thread(FileWatcher &fileWatcher)
  {
    HANDLE hDir = CreateFile(fileWatcher.GetDirectory().generic_wstring().c_str(),    // path to the directory
                             FILE_LIST_DIRECTORY,                                    // access (read/write) mode
                             FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, // share mode
                             NULL,                                                   // no child process
                             OPEN_EXISTING,                                          // only open existing
                             FILE_FLAG_BACKUP_SEMANTICS,                             // file attributes for directory handle
                             NULL                                                    // no template
    );

    wchar_t file[MAX_PATH];
    FILE_NOTIFY_INFORMATION buffer[1024];
    DWORD bytes;

    while (ReadDirectoryChangesW(
               hDir,           // handle to directory
               &buffer,        // read results buffer
               sizeof(buffer), // length of buffer
               FALSE,          // not monitoring subdirectories
               FILE_NOTIFY_CHANGE_SECURITY |
                   FILE_NOTIFY_CHANGE_CREATION |
                   FILE_NOTIFY_CHANGE_LAST_ACCESS |
                   FILE_NOTIFY_CHANGE_LAST_WRITE |
                   FILE_NOTIFY_CHANGE_SIZE |
                   FILE_NOTIFY_CHANGE_ATTRIBUTES |
                   FILE_NOTIFY_CHANGE_DIR_NAME |
                   FILE_NOTIFY_CHANGE_FILE_NAME, // filter conditions
               &bytes,                           // bytes returned
               NULL,                             // overlapped buffer
               NULL                              // completion routine
               ) &&
           fileWatcher.m_running.load())
    {
      FILE_NOTIFY_INFORMATION *information;
      information = (FILE_NOTIFY_INFORMATION *)((char *)buffer);
      wcscpy_s(file, L"");

      wcsncpy_s(file, information->FileName, information->FileNameLength / 2);

      file[information->FileNameLength / 2] = NULL;

      switch (buffer[0].Action)
      {
      case FILE_ACTION_ADDED:
        fileWatcher.OnFileAdded(file);
        break;
      case FILE_ACTION_MODIFIED:
        fileWatcher.OnFileChanged(file);
        break;
      case FILE_ACTION_REMOVED:
        fileWatcher.OnFileDeleted(file);
        break;
      case FILE_ACTION_RENAMED_OLD_NAME:
        fileWatcher.OnFileRenamed(file);
        break;
      case FILE_ACTION_RENAMED_NEW_NAME:
        fileWatcher.OnFileRenamed(file);
        break;
      }
      Sleep(1000);
    }

    CloseHandle(hDir);
  }
} // namespace DeltaEngine