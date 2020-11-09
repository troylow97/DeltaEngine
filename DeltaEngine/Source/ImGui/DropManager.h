#pragma once

#include <oleidl.h>
#include <shellapi.h>
#include <locale> 
#include <codecvt>
#include "Input/InputManager.h"
#include "Event/ApplicationEvent.h"
#include "Core/Utils/FileUtils.h"

namespace DeltaEngine
{
    class DropManager : public IDropTarget
    {
    private:
        bool _isDragged = false;
        bool _isDropped = false;
        bool _isInImGui = false;
        bool _isInPanel = false;
        bool _isReleased = false;
        bool _passingToPanel = false;

    public:
        // handling drop targets, letting others know
        HRESULT QueryInterface(REFIID riid, void** ppvObject)
        {
            if (riid == IID_IDropTarget)
            {
                *ppvObject = static_cast<IUnknown*>(this);
                return S_OK;
            }
            *ppvObject = NULL;
            return E_NOINTERFACE;
            //return S_OK;
        }
        unsigned long AddRef(void)
        {
            return 1;
        }
        unsigned long Release(void)
        {
            return 0;
        }

        // implementing the IDropTarget parts --------------------------------------------------------------------------------

        // when dragging the files into imgui view
        // indicates whether a drop can be accepted
        HRESULT DragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
        {
            if (pdwEffect == NULL)
            {
                return E_INVALIDARG;
            }
            // TODO: check whether we can handle this type of object at all and set *pdwEffect &= DROPEFFECT_NONE if not;
            /*
            if can handlle()
            {

            }
            */

            // flagging the application that files have been dragged from the OS into imgui
            _isInImGui = true;

            // trigger MouseDown for button 1 within ImGui

            *pdwEffect |= DROPEFFECT_COPY;
            return S_OK;
        }
        // when cursor is on imgui with the file
        //  provides target feedback to the user
        //  communicates the drop's effect to the DoDragDrop function 
        //  so it can communicate the effect of the drop back to the source
        HRESULT DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
        {
            if (pdwEffect == NULL)
            {
                return E_INVALIDARG;
            }
            // trigger MouseMove within ImGui, position is within pt.x and pt.y
            //if (InputManager::Get()->CurrentCameraPosition().point_x >= Camera::editorCamera->Min().x && InputManager::Get()->CurrentCameraPosition().point_x <= Camera::editorCamera->Max().x
            //    && InputManager::Get()->CurrentCameraPosition().point_y >= Camera::editorCamera->Min().y && InputManager::Get()->CurrentCameraPosition().point_y <= Camera::editorCamera->Max().y)
            //{
            //    _isInPanel = true;
            //    std::cout << "it is in camera panel!!!" << std::endl;
            //}

            env.eventManager->AddEvent(new ImGuiFileDragEvent);

            // grfKeyState contains flags for control, alt, shift etc

            *pdwEffect |= DROPEFFECT_COPY;
            return S_OK;
        }
        // when dragging the files from imgui view
        // removes target feedback and releases the data object
        HRESULT DragLeave(void)
        {
            _isInPanel = false;
            _isInImGui = false;

            env.eventManager->AddEvent(new ImGuiFileRemovingDragEvent);

            return S_OK;
        }
        // when releasing the mouse button so files drop into imgui
        // incorporates the source data into the target window, removes target feedback, and releases the data object
        HRESULT Drop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
        {
            if (pdwEffect == NULL)
            {
                return E_INVALIDARG;
            }
            *pdwEffect |= DROPEFFECT_COPY;
            // grfKeyState contains flags for control, alt, shift etc

            // render the data into medium using the data description in format
            FORMATETC format;
            STGMEDIUM medium;

            format.cfFormat = CF_HDROP;
            format.ptd = NULL;
            format.dwAspect = DVASPECT_CONTENT;
            format.lindex = -1;
            format.tymed = TYMED_HGLOBAL;
            medium.tymed = TYMED_HGLOBAL;

            HRESULT res = pDataObj->GetData(&format, &medium);
            HDROP drop = reinterpret_cast<HDROP>(medium.hGlobal); // 
            wchar_t* fileName = NULL;

            // 0xFFFFFFFF returns the count of files dropped
            unsigned int fileCount = DragQueryFile(drop, 0xFFFFFFFF, NULL, 512);
            unsigned int longestFileNameLength = 0;

            // to drag and drop multiple files
            for (unsigned int i = 0; i < fileCount; ++i)
            {
                // NULL returns the length of the path
                unsigned int fileNameLength = DragQueryFile(drop, i, NULL, 512) + 1;

                if (fileNameLength > longestFileNameLength)
                {
                    if (fileName != NULL)
                    {
                        free(fileName);
                    }
                    longestFileNameLength = fileNameLength;
                    fileName = (wchar_t*)malloc(longestFileNameLength * sizeof(*fileName));
                }
                DragQueryFile(drop, i, fileName, fileNameLength);

                std::wcout << fileName << std::endl;
             
                // provide data of files !!!!!!!!!!!!!!!!!!!!!
                std::wstring fileNamePath = fileName;
                std::replace(fileNamePath.begin(), fileNamePath.end(), '\\', '/'); // replace all '\' to '/'

                std::size_t index = fileNamePath.find_last_of(L"/\\"); 
                std::wstring newFileName;
                std::wstring newPathName = L"Tilemap/";
                for (size_t j = index; j < fileNamePath.length(); ++j)
                {
                    newFileName += fileNamePath[j];
                }
                newPathName += newFileName;
                FileUtils::CreateDir("Tilemap");
                FileUtils::CopyFileW(fileNamePath, newPathName);

                //env.pManager->load<Texture2D>(newFileName, newPathName);

                //env.pManager->SetLoader<Texture2D>(new ImGuiFileDragEventDone);
            }
            if (fileName != NULL)
            {
                free(fileName);
            }

            // releasing the data once done
            ReleaseStgMedium(&medium);

            // notify imgui that dragging of the files is done
            env.eventManager->AddEvent(new ImGuiFileDragEventDone);
            _isReleased = true;
            _isDragged = false;
            _isDropped = true;
            _isInImGui = false;
            _isInPanel = false;


            // trigger MouseUp for button 1 within ImGui

            return S_OK;
        }
    };
}