/**********************************************************************************
* \file   FileDialog.h
* \brief  This file contains the implementation of a FileDialog using win32 API
*
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include <optional>
#include <string>

namespace DeltaEngine::FileDialogs
{
  std::optional<std::string> OpenFile(const char* filter);
  std::optional<std::string> SaveFile(const char* filter);
} // namespace DeltaEngine::FileUtils
