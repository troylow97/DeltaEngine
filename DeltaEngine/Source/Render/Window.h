#pragma once
#include "DEpch.h"
#include "Core/Debugging/Logger/Log.h"

#define MAX_LOADSTRING 100
 
namespace DeltaEngine
{

class Window
{
  std::wstring m_title { L"Metamorphosis" };
  HWND m_hwndl;
  bool m_running;
  int m_width;
  int m_height;
  bool m_fullscreen;
  bool m_cursor;

public:

  Window() = default;
  ~Window() = default;

  void Init();
  void Update();
  void ShutDown();

  HWND GetHandle() const;

  void Height( int h );
  [[nodiscard]] int Height() const;

  void Width( int w );
  [[nodiscard]] int Width() const;

  void Fullscreen( bool f );
  [[nodiscard]] bool Fullscreen() const;

  void Cursor(bool c);
  [[nodiscard]] bool Cursor() const;

  void Running( bool r );
  [[nodiscard]] bool Running() const;

  void InitWindow();
private:

};

}