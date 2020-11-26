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
