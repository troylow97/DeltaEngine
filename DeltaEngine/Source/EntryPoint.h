#pragma once

#ifdef DE_PLATFORM_WINDOWS
using namespace DeltaEngine;
extern Application* DeltaEngine::CreateApplication();

int main(int argc, char** argv)
{
  auto app = CreateApplication();
  app->Run();
  delete app;
  DeltaEngine_CORE_INFO("APPLICATION EXIT");
}

#endif // DE_PLATFORM_WINDOWS
