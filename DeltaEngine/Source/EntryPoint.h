#pragma once

#include "DeltaEngine.h"

#ifdef DE_PLATFORM_WINDOWS
using namespace DeltaEngine;
extern DeltaEngine::Application* DeltaEngine::CreateApplication();

int main(int argc,char** argv)
{
	RenderModule::CreateWin32Window();

	auto app = DeltaEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif // DE_PLATFORM_WINDOWS

