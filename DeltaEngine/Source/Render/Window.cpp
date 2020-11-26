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

std::wstring to_wstring( std::string str )
{
  if ( str.empty() )
  {
    return std::wstring();
  }
  int num_chars = MultiByteToWideChar( CP_ACP, MB_ERR_INVALID_CHARS, str.c_str(), static_cast<int>( str.length() ),
                                       nullptr, 0 );
  std::wstring wstrTo;
  if ( num_chars )
  {
    wstrTo.resize( num_chars );
    if ( MultiByteToWideChar( CP_ACP, MB_ERR_INVALID_CHARS, str.c_str(), static_cast<int>( str.length() ), &wstrTo[0],
         num_chars ) )
    {
      return wstrTo;
    }
  }
  return std::wstring();
}

extern LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

namespace DeltaEngine
{
LRESULT WINAPI Win32WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{

  if ( ImGui_ImplWin32_WndProcHandler( hwnd, uMsg, wParam, lParam ) )
    return true;
  switch ( uMsg )
  {
    case WM_COMMAND:
    {
      int wmId = LOWORD( wParam );
      // Parse the menu selections:
      switch ( wmId )
      {
        case ID_FILE_EXIT:
          DestroyWindow( hwnd );
          break;
        default:
          return DefWindowProc( hwnd, uMsg, wParam, lParam );
      }
    }
    break;
    case WM_SIZE:
    {
      env.pWin->Width( LOWORD( lParam ) );
      env.pWin->Height( HIWORD( lParam ) );
    }
    break;
    case WM_QUIT:
    case WM_CLOSE:
    case WM_DESTROY:
    {
      env.pWin->Running( false );
      PostQuitMessage( 0 );
    }
    break;
    case WM_ACTIVATEAPP:
    {
      if ( wParam )
        env.pWin->Focus( true );
      else
        env.pWin->Focus( false );
    }
    break;
  }
  return DefWindowProc( hwnd, uMsg, wParam, lParam );
}



Window::Window( const std::string &title, int width, int height, bool fullscreen ) :
  m_title { to_wstring( title ) }, m_width { width }, m_height { height }, m_fullscreen { fullscreen }
{
}


void Window::Init()
{
  m_running = true;
  m_cursor = false;
  m_focus = true;
  InitWindow();
}

void Window::Update()
{
  MSG msg = {};

  if ( PeekMessage( &msg, nullptr, 0U, 0U, PM_REMOVE ) )
  {
    TranslateMessage( &msg );
    DispatchMessage( &msg );
  }

  if(m_focus)
  Profiler::Instance().Record( "Window" );
}

void Window::ShutDown()
{
  DestroyWindow( m_hwndl );
  RevokeDragDrop( m_hwndl );
  OleUninitialize();
}

HWND Window::GetHandle() const
{
  return m_hwndl;
}

void Window::Height( const int h )
{
  m_height = h;
}

int Window::Height() const
{
  return m_height;
}

void Window::Width( const int w )
{
  m_width = w;
}

int Window::Width() const
{
  return m_width;
}

void Window::Fullscreen( const bool f )
{
  m_fullscreen = f;
}

bool Window::Fullscreen() const
{
  return m_fullscreen;
}

void Window::Cursor( const bool c )
{
  m_cursor = c;
}

bool Window::Cursor() const
{
  return m_cursor;
}

void Window::Running( const bool r )
{
  m_running = r;
}

bool Window::Running() const
{
  return m_running;
}

void Window::Focus( const bool f )
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
  windowClass.hInstance = GetModuleHandle( nullptr );
  windowClass.lpszClassName = L"DeltaEngine";
  windowClass.hCursor = LoadCursor( nullptr, IDC_ARROW );
  windowClass.lpszMenuName = MAKEINTRESOURCEW( IDR_MENU1 );

  if ( !RegisterClass( &windowClass ) )
  {
    DeltaEngine_CORE_ERROR( "ERROR: Couldn't register window class!" );
  }

  HRESULT oleResult = OleInitialize( nullptr );
  (void) oleResult;
  m_hwndl = CreateWindowEx( 0, windowClass.lpszClassName, m_title.c_str(),
                            WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, m_width, m_height,
                            nullptr, nullptr, windowClass.hInstance, nullptr );

  if ( !m_hwndl )
  {
    DeltaEngine_CORE_ERROR( "ERROR: Couldn't create window!" );
  }

  if ( m_fullscreen && m_hwndl )
    SetWindowPos( m_hwndl, HWND_TOPMOST, 0, 0, GetSystemMetrics( SM_CXSCREEN ), GetSystemMetrics( SM_CYSCREEN ), 0L );

  ShowWindow( GetConsoleWindow(), SW_SHOW );

  RegisterDragDrop( m_hwndl, &dropManager );
}
}
