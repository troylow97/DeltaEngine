#include "DropManager.h"
#include <shellapi.h>
#include <locale>

#include "Core/GlobalStruct.h"
#include "Core/Debugging/Logger/Log.h"
#include "Event/ApplicationEvent.h"

namespace DeltaEngine
{

unsigned long DropManager::AddRef()
{
  return 1;
}

unsigned long DropManager::Release()
{
  return 0;
}

HRESULT DropManager::QueryInterface( REFIID riid, void **ppvObject )
{
  if ( riid == IID_IDropTarget )
  {
    *ppvObject = static_cast<IUnknown *>( this );
    return S_OK;
  }
  *ppvObject = nullptr;
  return E_NOINTERFACE;
}

// implementing the IDropTarget parts --------------------------------------------------------------------------------

// when dragging the files into imgui view
// indicates whether a drop can be accepted
HRESULT DropManager::DragEnter( IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect )
{
  UNREFERENCED_PARAMETER( pDataObj );
  UNREFERENCED_PARAMETER( grfKeyState );
  UNREFERENCED_PARAMETER( pt );
  if ( pdwEffect == nullptr )
  {
    return E_INVALIDARG;
  }
  // TODO: check whether we can handle this type of object at all and set *pdwEffect &= DROPEFFECT_NONE if not;
  /*
  if can handlle()
  {

  }
  */

  // trigger MouseDown for button 1 within ImGui
  *pdwEffect &= DROPEFFECT_COPY;
  return S_OK;
}

// when cursor is on imgui with the file
//  provides target feedback to the user
//  communicates the drop's effect to the DoDragDrop function 
//  so it can communicate the effect of the drop back to the source
HRESULT DropManager::DragOver( DWORD grfKeyState, POINTL pt, DWORD *pdwEffect )
{
  UNREFERENCED_PARAMETER( grfKeyState );
  UNREFERENCED_PARAMETER( pt );

  if ( pdwEffect == nullptr )
    return E_INVALIDARG;

  env.eventManager->AddEvent( new ImGuiFileDragEvent );

  // grfKeyState contains flags for control, alt, shift etc

  *pdwEffect &= DROPEFFECT_COPY;
  return S_OK;
}

// when dragging the files from imgui view
// removes target feedback and releases the data object
HRESULT DropManager::DragLeave()
{
  env.eventManager->AddEvent( new ImGuiFileRemovingDragEvent );
  return S_OK;
}

// when releasing the mouse button so files drop into imgui
// incorporates the source data into the target window, removes target feedback, and releases the data object
HRESULT DropManager::Drop( IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect )
{
  UNREFERENCED_PARAMETER( grfKeyState );
  UNREFERENCED_PARAMETER( pt );

  if ( pdwEffect == nullptr )
  {
    return E_INVALIDARG;
  }
  *pdwEffect &= DROPEFFECT_COPY;
  // grfKeyState contains flags for control, alt, shift etc

  // render the data into medium using the data description in format
  FORMATETC format;
  STGMEDIUM medium;

  format.cfFormat = CF_HDROP;
  format.ptd = nullptr;
  format.dwAspect = DVASPECT_CONTENT;
  format.lindex = -1;
  format.tymed = TYMED_HGLOBAL;
  medium.tymed = TYMED_HGLOBAL;

  HRESULT res = pDataObj->GetData( &format, &medium );
  (void) res;
  HDROP drop = reinterpret_cast<HDROP>( medium.hGlobal ); // 

  // 0xFFFFFFFF returns the count of files dropped
  const unsigned int fileCount = DragQueryFile( drop, 0xFFFFFFFF, nullptr, 512 );

  // to drag and drop multiple files
  for ( unsigned int i = 0; i < fileCount; ++i )
  {
    // NULL returns the length of the path
    unsigned int fileNameLength = DragQueryFile( drop, i, nullptr, 512 ) + 1;
    wchar_t buffer[512];
    DragQueryFile( drop, i, buffer, fileNameLength );
    DeltaEngine_CORE_INFO( "Dragged File - {}",
                           drop_vec.emplace_back( std::filesystem::path( buffer ) ).generic_string() );
  }


  // releasing the data once done
  ReleaseStgMedium( &medium );

  // notify imgui that dragging of the files is done
  env.eventManager->AddEvent( new ImGuiFileDragEventDone );

  // trigger MouseUp for button 1 within ImGui

  return S_OK;
}
}