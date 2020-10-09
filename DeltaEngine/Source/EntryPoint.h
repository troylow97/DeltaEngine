#pragma once

#ifdef DE_PLATFORM_WINDOWS
using namespace DeltaEngine;
extern DeltaEngine::Application* DeltaEngine::CreateApplication();

int main(int argc,char** argv)
{
	auto app = DeltaEngine::CreateApplication();
	app->Run();
	delete app;
	DeltaEngine_CORE_TRACE( "EXIT" );
}

#endif // DE_PLATFORM_WINDOWS

