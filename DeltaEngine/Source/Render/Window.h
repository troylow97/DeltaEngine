#pragma once
#include "DEpch.h"
#include "Core/Debugging/Logger/Log.h"
#include <string>
#define MAX_LOADSTRING 100

namespace DeltaEngine
{
  class Window
  {
    HWND m_hwndl{nullptr};
    std::wstring m_title;
    int m_width;
    int m_height;
    bool m_running;
    bool m_focus;
    bool m_fullscreen;
    bool m_cursor;

  public:

    Window() = delete;
    Window(const std::string& title, int width, int height, bool fullscreen);
    ~Window() = default;

    void Init();
    void Update();
    void Shutdown();

    HWND GetHandle() const;

    void Height(int h);
    [[nodiscard]] int Height() const;

    void Width(int w);
    [[nodiscard]] int Width() const;

    void Fullscreen(bool f);
    [[nodiscard]] bool Fullscreen() const;

    void Cursor(bool c);
    [[nodiscard]] bool Cursor() const;

    void Running(bool r);
    [[nodiscard]] bool Running() const;

    void Focus(bool f);
    [[nodiscard]] bool Focus() const;

    void InitWindow();
  };
}
