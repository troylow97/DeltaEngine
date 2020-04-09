#pragma once

#ifdef DE_PLATFORM_WINDOWS

extern DeltaEngine::Application* DeltaEngine::CreateApplication();

int main(int argc,char** argv)
{
	auto app = DeltaEngine::CreateApplication();
	app->Run();
	delete app;

}

#endif // DE_PLATFORM_WINDOWS

