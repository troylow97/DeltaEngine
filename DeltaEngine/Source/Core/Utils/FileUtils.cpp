#include "FileUtils.h"
#include "Core/Debugging/Logger/Log.h"
#include <fstream>

namespace DeltaEngine::FileUtils
{

//****************************************************************************
// FileSystem Root / Temp Path Utilities
//****************************************************************************

void Root(fs::path path)
{
  if (DirExists(path))
    fs::current_path(path);
  else
  {
    DeltaEngine_CORE_ERROR( "Invalid File:Path" );
    abort();
  }
}

fs::path Root()
{
  return fs::current_path();
}

fs::path TempDir()
{
  return fs::temp_directory_path();
}

//****************************************************************************
// FileSystem Directories Utilities
//****************************************************************************
bool CreateDir(fs::path dir_name)
{
  return fs::create_directories(dir_name);
}

void CopyDir(fs::path source, fs::path dest)
{
  if (DirExists(source))
  {
    constexpr fs::copy_options options = fs::copy_options::update_existing |
      fs::copy_options::recursive;

    fs::copy(source, dest, options);
  }
  else
    DeltaEngine_CORE_ERROR( "Source directory cannot be found" );
}

bool DeleteDir(fs::path dir_name)
{
  if (DirExists(dir_name))
  {
    std::error_code ec;
    bool result = fs::remove(dir_name, ec);
    if (ec)
      DeltaEngine_CORE_ERROR( "{}", ec.message() );
    return result;
  }
  else
    DeltaEngine_CORE_ERROR( "Directory cannot be found" );
  return false;
}

size_t DeleteAllDir(fs::path dir_name)
{
  if (DirExists(dir_name))
  {
    std::error_code ec;
    size_t result = fs::remove_all(dir_name, ec);
    if (ec)
      DeltaEngine_CORE_ERROR( "{}", ec.message() );
    return result;
  }
  else
    DeltaEngine_CORE_ERROR( "Directory cannot be found" );
  return 0;
}

bool DirExists(fs::path dir_name)
{
  return fs::exists(dir_name) &&
    fs::is_directory(dir_name);
}

std::vector<fs::directory_entry> DirList(fs::path dir)
{
  std::vector<fs::directory_entry> dir_list;
  for (auto &ref : fs::directory_iterator(dir))
    if (fs::is_directory(ref))
      dir_list.push_back(ref);

  return dir_list;
}

//****************************************************************************
// FileSystem Files Utilities
//****************************************************************************

fs::path CreateFile(fs::path filename)
{
  if (!filename.parent_path().empty())
    if (!DirExists(filename.parent_path()))
    {
      DeltaEngine_CORE_ERROR( "Directory cannot be found" );
      return fs::path();
    }

  std::ofstream dst(filename, std::ios::binary | std::ios::app);
  dst.close();
  return fs::path(filename);
}

bool DeleteFile(fs::path filename)
{
  if (FileExists(filename))
    return fs::remove(filename);
  else
    DeltaEngine_CORE_ERROR( "Directory cannot be found" );

  return false;
}

void CopyFile(fs::path source, fs::path dest)
{
  if (FileExists(source))
  {
    constexpr fs::copy_options options = fs::copy_options::update_existing;

    fs::copy_file(source, dest, options);
  }
  else
    DeltaEngine_CORE_ERROR( "Source file cannot be found" );
}

bool FileExists(fs::path filename)
{
  return fs::exists(filename) && fs::is_regular_file(filename);
}

size_t FileSize(fs::path filename)
{
  return fs::file_size(filename);
}

std::vector<fs::path> FileList(fs::path dir)
{
  std::vector<fs::path> file_list;
  for (auto &ref : fs::directory_iterator(dir))
    if (fs::is_regular_file(ref))
      file_list.push_back(ref.path());

  return file_list;
}

} // namespace DeltaEngine::FileUtils