#pragma once

#ifdef DE_PLATFORM_WINDOWS
using namespace DeltaEngine;
extern DeltaEngine::Application* DeltaEngine::CreateApplication();

int main(int argc,char** argv)
{
	DeltaEngine::Log::Init();
	DeltaEngine::Log::GetCoreLogger()->info("Hello, {}!", "World");

	DeltaEngine_CORE_WARN("Initialised Log!");

	RenderModule::CreateWin32Window();
	
	//RenderModule::openGLSystem = new RenderModule::OpenGLSystem();
	//RenderModule::openGLSystem.Init();

	auto app = DeltaEngine::CreateApplication();
	app->Run();
	delete app;

	//RenderModule::openGLSystem->Exit();
	//delete RenderModule::openGLSystem;


}

#endif // DE_PLATFORM_WINDOWS

