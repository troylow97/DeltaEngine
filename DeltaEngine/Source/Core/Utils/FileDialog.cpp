/**********************************************************************************
* \file   FileDialog.cpp
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
#include "FileDialog.h"
#include "Core/GlobalStruct.h"
#include "Render/Window.h"
#include <commdlg.h>
#include "FileUtils.h"

namespace DeltaEngine::FileDialogs
{
  std::optional<std::string> OpenFile(const char* filter)
  {
    OPENFILENAMEA ofn;
    CHAR szFile[260] = {0};
    ZeroMemory(&ofn, sizeof( OPENFILENAME ));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = GetEnv().pWin->GetHandle();
    ofn.lpstrInitialDir =
      ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameA(&ofn) == TRUE)
      return ofn.lpstrFile;
    return std::nullopt;
  }

  std::optional<std::string> SaveFile(const char* filter)
  {
    OPENFILENAMEA ofn;
    CHAR szFile[260] = {0};
    ZeroMemory(&ofn, sizeof( OPENFILENAME ));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = GetEnv().pWin->GetHandle();
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    // Sets the default extension by extracting it from the filter
    ofn.lpstrDefExt = strchr(filter, '\0') + 1;

    if (GetSaveFileNameA(&ofn) == TRUE)
      return ofn.lpstrFile;
    return std::nullopt;
  }
} // namespace DeltaEngine::FileUtils
