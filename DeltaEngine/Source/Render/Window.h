#pragma once
#include "DEpch.h"
#include "Core/Debugging/Logger/Log.h"

#define MAX_LOADSTRING 100

namespace DeltaEngine::RenderModule
{
  extern HWND mainHWND;
  extern int width, height;

  void DE_API CreateWin32Window();
}
