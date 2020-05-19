#pragma once
#include "DEpch.h"
#include "../Core/Log.h"

#define MAX_LOADSTRING 100

namespace DeltaEngine
{
    namespace RenderModule
    {
        extern HWND mainHWND;
        extern int width, height;

        void DE_API CreateWin32Window();
    };
}
