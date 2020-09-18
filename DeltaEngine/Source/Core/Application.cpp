#include "DEpch.h"
#include "Application.h"
#include "../Render/GraphicsManager.h"
#include "../Render/SpriteRenderer.h"
#include "Core/InputSystem.h"

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

		//Input::InputSystem::get()->addListener(this);

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

			Input::InputSystem::get()->inputUpdate();

			//Input::InputSystem::get()->isKeyTriggered();
			if (Input::InputSystem::get()->isKeyTriggered(DEVK_A))
				std::cout << "A triggered here" << std::endl;

			if (Input::InputSystem::get()->isKeyPressed(DEVK_A))
				std::cout << "A pressed here" << std::endl;
			
			if (Input::InputSystem::get()->isKeyReleased(DEVK_A))
				std::cout << "A released here" << std::endl;

			if (Input::InputSystem::get()->isMouseTriggered(DEVK_LBUTTON))
				std::cout << "lmb is triggered" << std::endl;

			if (Input::InputSystem::get()->isMouseTriggered(DEVK_RBUTTON))
				std::cout << "rmb is triggered" << std::endl;

			if (Input::InputSystem::get()->isMousePressed(DEVK_LBUTTON))
				std::cout << "lmb is pressed" << std::endl;

			if (Input::InputSystem::get()->isMousePressed(DEVK_RBUTTON))
				std::cout << "rmb is pressed" << std::endl;

			if (Input::InputSystem::get()->isMouseReleased(DEVK_LBUTTON))
				std::cout << "lmb is released" << std::endl;

			if (Input::InputSystem::get()->isMouseReleased(DEVK_RBUTTON))
				std::cout << "rmb is released" << std::endl;
		}
		RenderModule::openGLSystem->Exit();
		delete RenderModule::openGLSystem;
	}
}
