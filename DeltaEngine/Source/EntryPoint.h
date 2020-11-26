#pragma once

#ifdef DE_PLATFORM_WINDOWS
using namespace DeltaEngine;
extern Application* DeltaEngine::CreateApplication();



// debug_new.cpp
// compile by using: cl /EHsc /W4 /D_DEBUG /MDd debug_new.cpp
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

int main(int argc, char** argv)
{
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);

  auto app = CreateApplication();
  app->Run();
  delete app;
  DeltaEngine_CORE_INFO("APPLICATION EXIT");
}

#endif // DE_PLATFORM_WINDOWS
