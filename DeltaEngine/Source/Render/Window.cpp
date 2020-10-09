#include "DEpch.h"
#include "Render/Window.h"
#include "../resource.h"
#include "Core/Debugging/Logger/Log.h"
#include <examples/imgui_impl_win32.h>
#include "Core/GlobalStruct.h"

#include <codecvt>
#include <locale>

using convert_t = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_t, wchar_t> strconverter;

std::wstring to_wstring(std::string str)
{
  return strconverter.from_bytes( str );
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
      RECT rect;
      if ( GetWindowRect( hwnd, &rect ) )
      {
        env.pWin->Width( rect.right - rect.left );
        env.pWin->Height( rect.bottom - rect.top );
      }
    }
    break;
    case WM_DESTROY:
    {
      env.pWin->Running( false );
      PostQuitMessage( 0 );
    }
    break;
  }
  return DefWindowProc( hwnd, uMsg, wParam, lParam );
}

Window::Window(std::string title, int width, int height) :
  m_title { to_wstring( title ) }, m_width { width }, m_height { height }
{
  
}


void Window::Init()
{
  m_running = true;
  m_fullscreen = m_cursor = false;
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
}

void Window::ShutDown()
{
  DestroyWindow( m_hwndl );
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

void Window::InitWindow()
{

  // Register window class and create window
  WNDCLASS windowClass = {};
  windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  windowClass.lpfnWndProc = Win32WindowProc;
  windowClass.hInstance = GetModuleHandle( NULL );
  windowClass.lpszClassName = L"DeltaEngine";
  windowClass.hCursor = LoadCursor( NULL, IDC_ARROW );
  windowClass.lpszMenuName = MAKEINTRESOURCEW( IDR_MENU1 );

  if ( !RegisterClass( &windowClass ) )
    DeltaEngine_CORE_ERROR( "ERROR: Couldn't register window class!" );

  m_hwndl = CreateWindowEx( 0, windowClass.lpszClassName, m_title.c_str(),
                            WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, m_width, m_height,
                            0, 0, windowClass.hInstance, 0 );

  if ( !m_hwndl )
    DeltaEngine_CORE_ERROR( "ERROR: Couldn't create window!" );

  ShowWindow( GetConsoleWindow(), SW_SHOW );
}

}