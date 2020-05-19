#include "DEpch.h"
#include "Render/Window.h"
#include "../resource.h"
#include "../Core/Log.h"

namespace DeltaEngine
{
    namespace RenderModule
    {
        HWND mainHWND;
        int width = 800, height = 600;

        LRESULT WINAPI Win32WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            switch (uMsg)
            {
            case WM_COMMAND:
            {
                int wmId = LOWORD(wParam);
                // Parse the menu selections:
                switch (wmId)
                {
                case ID_FILE_EXIT:
                    DestroyWindow(hwnd);
                    break;
                default:
                    return DefWindowProc(hwnd, uMsg, wParam, lParam);
                }
            }
            break;
            case WM_DESTROY:
            {
                PostQuitMessage(0);
            }
            break;
            }
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
        void CreateWin32Window()
        {
            // Register window class and create window
            WNDCLASS windowClass = {};
            windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
            windowClass.lpfnWndProc = Win32WindowProc;
            windowClass.hInstance = GetModuleHandle(NULL);
            windowClass.lpszClassName = L"Delta Engine";
            windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
            windowClass.lpszMenuName = MAKEINTRESOURCEW(IDR_MENU1);

            if (!RegisterClass(&windowClass))
                DeltaEngine_CORE_ERROR("ERROR: Couldn't register window class!");

            mainHWND = CreateWindowEx(0, windowClass.lpszClassName, L"Delta Engine",
                WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, width, height,
                0, 0, windowClass.hInstance, 0);

            if (!mainHWND)
                DeltaEngine_CORE_ERROR("ERROR: Couldn't create window!");

            ShowWindow(GetConsoleWindow(), SW_SHOW);
        }
    }
}
