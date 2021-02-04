/**********************************************************************************
* \file   IFileWatcherListener.h
* \brief  This file contains the defintion for the filewatcher listener interface
*
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include <filesystem>

namespace DeltaEngine
{
  struct IFileWatcherListener
  {
    virtual void OnFileAdded(std::filesystem::path path) = 0;
    virtual void OnFileDeleted(std::filesystem::path path) = 0;
    virtual void OnFileChanged(std::filesystem::path path) = 0;
    virtual void OnFileRenamed(std::filesystem::path path) = 0;
    virtual ~IFileWatcherListener() = default;
  };
} // namespace DeltaEngine
