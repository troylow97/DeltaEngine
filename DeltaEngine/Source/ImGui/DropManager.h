/**********************************************************************************
* \file   DropManager.h.cpp
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
#include <oleidl.h>

namespace DeltaEngine
{
class DropManager : public IDropTarget
{
public:
  inline static std::vector<std::filesystem::path> drop_vec;
  unsigned long AddRef() override;
  unsigned long Release() override;

  HRESULT QueryInterface( REFIID riid, void **ppvObject ) override;
  HRESULT DragEnter( IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect ) override;
  HRESULT DragOver( DWORD grfKeyState, POINTL pt, DWORD *pdwEffect ) override;
  HRESULT DragLeave() override;
  HRESULT Drop( IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect ) override;
};
}
