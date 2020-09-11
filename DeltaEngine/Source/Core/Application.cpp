#include "DEpch.h"
#include "Application.h"
#include "../Render/GraphicsManager.h"
#include "../Render/SpriteRenderer.h"

/*-----------------------------------
#include "Event/ApplicationEvent.h"
#include "Log.h"
-----------------------------------*/
namespace DeltaEngine
{
	std::vector<SpriteRenderer*> sprites;

	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		//sf::Event event;
		// a lot of this should be moved to a function in GraphicsManager later

		RenderModule::openGLSystem = new RenderModule::OpenGLSystem();
		RenderModule::openGLSystem->Init();

		sprites.push_back(new SpriteRenderer());

		Input::InputSystem input_system; // 

		MSG msg = {};
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				continue;
			}
			RenderModule::openGLSystem->TestRender(sprites);
			input_system.left();
		}
		RenderModule::openGLSystem->Exit();
		delete RenderModule::openGLSystem;
	}
}
