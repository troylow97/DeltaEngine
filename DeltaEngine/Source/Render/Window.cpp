#include "DEpch.h"
#include "Window.h"
#include "Camera.h"
#include "../resource.h"
#include "Core/Debugging/Logger/Log.h"
#include <examples/imgui_impl_win32.h>
#include "Core/GlobalStruct.h"
#include "ImGui/DropManager.h"

#include <locale>
#include <windowsx.h>
#include "Core/Debugging/Profiler/Profiler.h"
#include "Core/GameClock/EngineClock.h"

std::wstring to_wstring(std::string str)
{
  if (str.empty())
  {
    return std::wstring();
  }
  int num_chars = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, str.c_str(), static_cast<int>(str.length()),
                                      nullptr, 0);
  std::wstring wstrTo;
  if (num_chars)
  {
    wstrTo.resize(num_chars);
    if (MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, str.c_str(), static_cast<int>(str.length()), &wstrTo[0],
                            num_chars))
    {
      return wstrTo;
    }
  }
  return std::wstring();
}

WINDOWPLACEMENT wpc;
LONG HWNDStyle = 0;
LONG HWNDStyleEx = 0;

void DeltaEngine::Window::Fullscreen( )
{
  if ( !m_fullscreen  )
  {
    m_fullscreen = true;
    GetWindowPlacement( m_hwndl, &wpc );
    if ( HWNDStyle == 0 )
      HWNDStyle = GetWindowLong( m_hwndl, GWL_STYLE );
    if ( HWNDStyleEx == 0 )
      HWNDStyleEx = GetWindowLong( m_hwndl, GWL_EXSTYLE );

    LONG NewHWNDStyle = HWNDStyle;
    NewHWNDStyle &= ~WS_BORDER;
    NewHWNDStyle &= ~WS_DLGFRAME;
    NewHWNDStyle &= ~WS_THICKFRAME;

    LONG NewHWNDStyleEx =HWNDStyleEx;
    NewHWNDStyleEx &= ~WS_EX_WINDOWEDGE;

    SetWindowLong( m_hwndl, GWL_STYLE, NewHWNDStyle | WS_POPUP );
    SetWindowLong( m_hwndl, GWL_EXSTYLE, NewHWNDStyleEx | WS_EX_TOPMOST );
    ShowWindow( m_hwndl, SW_SHOWMAXIMIZED );
  }
  else
  {
    m_fullscreen = false;
    SetWindowLong( m_hwndl, GWL_STYLE, HWNDStyle );
    SetWindowLong( m_hwndl, GWL_EXSTYLE, HWNDStyleEx );
    ShowWindow( m_hwndl, SW_SHOWNORMAL );
    SetWindowPlacement( m_hwndl, &wpc );
  }
}


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace DeltaEngine
{
  LRESULT WINAPI Win32WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
      return true;
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
    case WM_SIZE:
      {
        env.pWin->Width(LOWORD(lParam));
        env.pWin->Height(HIWORD(lParam));
      }
      break;
    case WM_QUIT:
    case WM_CLOSE:
    case WM_DESTROY:
      {
        env.pWin->Running(false);
        PostQuitMessage(0);
      }
      break;
    case WM_ACTIVATE:
    case WM_ACTIVATEAPP:
      {
        if (wParam)
          env.pWin->Focus(true);
        else
          env.pWin->Focus(false);
      }
      break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }


  Window::Window(const std::string& title, int width, int height, bool fullscreen) :
    m_title{to_wstring(title)}, m_width{width}, m_height{height}, m_running{true}, m_focus{true}, m_fullscreen{fullscreen},
    m_cursor{false}
  {
  }

  void Window::Init()
  {
    DeltaEngine_CORE_INFO("Initializing Window...");
    InitWindow();
    DeltaEngine_CORE_INFO("Initializing Window successful");
  }

  void Window::Update()
  {
#ifdef DE_EDITOR
    std::wstringstream wss;
    wss << m_title << L", FPS - " << env.pClock->FrameCount();
    SetWindowText( m_hwndl, wss.str().c_str());
#endif

    MSG msg = {};

    if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    if (m_focus)
      Profiler::Instance().Record("Window");
  }

  void Window::Shutdown()
  {
    DeltaEngine_CORE_INFO( "Shutting down Window..." );
    DestroyWindow(m_hwndl);
    RevokeDragDrop(m_hwndl);
    OleUninitialize();
    DeltaEngine_CORE_INFO( "Shutting down Window successful" );
  }

  HWND Window::GetHandle() const
  {
    return m_hwndl;
  }

  void Window::Height(const int h)
  {
    m_height = h;
  }

  int Window::Height() const
  {
    return m_height;
  }

  void Window::Width(const int w)
  {
    m_width = w;
  }

  int Window::Width() const
  {
    return m_width;
  }

  void Window::Fullscreen(const bool f)
  {
    m_fullscreen = f;
  }

  bool Window::Fullscreen() const
  {
    return m_fullscreen;
  }

  void Window::Cursor(const bool c)
  {
    m_cursor = c;
  }

  bool Window::Cursor() const
  {
    return m_cursor;
  }

  void Window::Running(const bool r)
  {
    m_running = r;
  }

  bool Window::Running() const
  {
    return m_running;
  }

  void Window::Focus(const bool f)
  {
    m_focus = f;
  }

  bool Window::Focus() const
  {
    return m_focus;
  }

  DropManager dropManager;

  void Window::InitWindow()
  {
    // Register window class and create window
    WNDCLASS windowClass = {};
    windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    windowClass.lpfnWndProc = Win32WindowProc;
    windowClass.hInstance = GetModuleHandle(nullptr);
    windowClass.lpszClassName = L"DeltaEngine";
    windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    windowClass.lpszMenuName = MAKEINTRESOURCEW(IDR_MENU1);

    if (!RegisterClass(&windowClass))
    {
      DeltaEngine_CORE_ERROR("ERROR: Couldn't register window class!");
    }

    HRESULT oleResult = OleInitialize(nullptr);
    (void)oleResult;
    m_hwndl = CreateWindowEx(0, windowClass.lpszClassName, m_title.c_str(),
                             WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, m_width, m_height,
                             nullptr, nullptr, windowClass.hInstance, nullptr);

    if (!m_hwndl)
    {
      DeltaEngine_CORE_ERROR("ERROR: Couldn't create window!");
    }

    if (m_fullscreen && m_hwndl)
    {
      m_fullscreen = !m_fullscreen;
      Fullscreen();
    }

#ifdef DE_EDITOR
    ShowWindow(GetConsoleWindow(), SW_SHOW);
#else
    ShowWindow( GetConsoleWindow(), SW_SHOW );
#endif

    RegisterDragDrop(m_hwndl, &dropManager);

  }

  Point Window::ClientTopLeft()
  {
    POINT p{};
    ClientToScreen( m_hwndl, &p );
    return { static_cast<float>( p.x ), static_cast<float>( p.y ) };
  }

  Point Window::ClientRect()
  {
    RECT r {};
    GetClientRect( m_hwndl, &r );
    return { static_cast<float>(r.right), static_cast<float>(r.bottom) };
  }
}
