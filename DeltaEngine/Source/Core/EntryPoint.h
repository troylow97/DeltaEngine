#pragma once

#ifdef DE_PLATFORM_WINDOWS

extern DeltaEngine::Application* DeltaEngine::CreateApplication();

int main(int argc,char** argv)
{
	DeltaEngine::Log::Init();
	DeltaEngine::Log::GetCoreLogger()->info("Hello, {}!", "World");

	//DeltaEngine_CORE_WARN("Initialised Log!");

	auto app = DeltaEngine::CreateApplication();
	app->Run();
	delete app;
	

}

#endif // DE_PLATFORM_WINDOWS

