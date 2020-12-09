/**********************************************************************************
* \file   DirectoryListener.cpp
* \brief  This file contains the implementation for the Directory Listener
*         Directory Listener is responsible for updating the cached directory
*         reference stored in the system directory
*
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "IFileWatcherListener.h"

namespace DeltaEngine
{
  struct DirectoryListener : IFileWatcherListener
  {
    void OnFileAdded(std::filesystem::path path) override;
    void OnFileDeleted(std::filesystem::path path) override;
    void OnFileChanged(std::filesystem::path path) override;
    void OnFileRenamed(std::filesystem::path path) override;
  };
}
