#pragma once

#ifdef DE_PLATFORM_WINDOWS
using namespace DeltaEngine;
extern DeltaEngine::Application* DeltaEngine::CreateApplication();

int main(int argc,char** argv)
{
	//RenderModule::openGLSystem = new RenderModule::OpenGLSystem();

	auto app = DeltaEngine::CreateApplication();
	app->Run();
	delete app;
	std::cout << "Destry" << std::endl;
	DeltaEngine_CORE_TRACE( "EXIT" );
	//RenderModule::openGLSystem->Exit();
	//delete RenderModule::openGLSystem;


}

#endif // DE_PLATFORM_WINDOWS

