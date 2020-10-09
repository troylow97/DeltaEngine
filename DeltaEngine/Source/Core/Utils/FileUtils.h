#pragma once
#include "DE_API.h"
#include <filesystem>
#include <vector>


namespace DeltaEngine::FileUtils
{

namespace fs = std::filesystem;

// Root Directory Path
void Root( fs::path path );
fs::path Root();

// Temporary Directory Path
fs::path TempDir();

// Managing Directory
bool CreateDir( fs::path dir_name );
void CopyDir( fs::path source, fs::path dest );
bool DeleteDir( fs::path dir_name );
size_t DeleteAllDir( fs::path dir_name );
bool DirExists( fs::path dir_name );
std::vector<fs::directory_entry> DirList( fs::path dir = fs::current_path() );

// Managing Files
fs::path CreateFile( fs::path filename );
bool DeleteFile( fs::path filename );
void CopyFile( fs::path source, fs::path dest );
bool FileExists( fs::path filename );
size_t FileSize( fs::path filename );
std::vector<fs::path> FileList( fs::path dir = fs::current_path() );

// TBC
// Open / Close files
// Read / Write stream for files
} // namespace DeltaEngine::FileUtils