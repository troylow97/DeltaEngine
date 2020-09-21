#pragma once
#include "DE_API.h"
#include <filesystem>
#include <vector>


namespace DeltaEngine::FileUtils
{

static std::string rootPath {"./"};

void SetRoot( std::string path );
std::string GetRoot();

bool AddDir( std::string dir_name );
bool RemoveDir( std::string dir_name );
bool DirExist( std::string dir_name );
std::vector<std::string> DirList();

bool AddFile( std::string filename );
bool RemoveFile( std::string filename);
void CopyFile( std::string source, std::string dest );
bool FileExist( std::string filename );
std::vector<std::string> FileList();

// TBC
// Open / Close files
// Read / Write stream for files 
}