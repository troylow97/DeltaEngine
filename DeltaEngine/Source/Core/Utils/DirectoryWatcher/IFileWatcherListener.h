/**********************************************************************************
* \file   IFileWatcherListener.h
* \brief  The file contains BLAHBLAHBLAH
* \author Chin, Clara,   X% Code Contribution
* \author Low, Troy,     X% Code Contribution
* \author Ong, Graeme,   X% Code Contribution
* \author Tan, Tong Wee, X% Code Contribution
*
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
    virtual void OnFileAdded(const std::filesystem::path path) = 0;
    virtual void OnFileDeleted(const std::filesystem::path path) = 0;
    virtual void OnFileChanged(const std::filesystem::path path) = 0;
    virtual void OnFileRenamed(const std::filesystem::path path) = 0;
    virtual ~IFileWatcherListener() = default;
  };
} // namespace DeltaEngine
