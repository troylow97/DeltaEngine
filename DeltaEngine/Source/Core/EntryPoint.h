#pragma once

#ifdef DE_PLATFORM_WINDOWS

#include "Render/Window.h"
#include "Render/OpenGLSystem.h"

extern DeltaEngine::Application* DeltaEngine::CreateApplication();
Graphics::OpenGLSystem* openGLSystem;

int main(int argc,char** argv)
{
	DeltaEngine::Log::Init();
	DeltaEngine::Log::GetCoreLogger()->info("Hello, {}!", "World");

	DeltaEngine_CORE_WARN("Initialised Log!");

	Window::Create();
	//openGLSystem = Graphics::CreateOpenGLSystem();
	//openGLSystem->Init();

	auto app = DeltaEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif // DE_PLATFORM_WINDOWS

