#include "DEpch.h"
#include "Application.h"
#include "../Render/GraphicsManager.h"
#include "../Render/SpriteRenderer.h"
#include "Event/MouseEvent.h"
#include "Event/ApplicationEvent.h"
#include "Physics/Collision.h"
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

	bool test_event_function(Event& e)
	{
		std::cout << "event" << std::endl;

		return true;
	}

	void Application::Run()
	{
		//sf::Event event;
		// a lot of this should be moved to a function in GraphicsManager later

		RenderModule::openGLSystem = new RenderModule::OpenGLSystem();
		RenderModule::openGLSystem->Init();

		sprites.push_back(new SpriteRenderer());
		//InputSystem input_manager;
		EventManager event_manager;
		//input_manager.testFunc();
		MSG msg = {};
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				continue;
			}
			//KeyPressedEvent keyevent{3,69};
			//EventDispatcher dispatcher(e);

			RenderModule::openGLSystem->TestRender(sprites);
			Input::InputSystem input_system;
			

			//if (keyevent.isHandled)
			//	break;

			//dispatcher.Dispatch<KeyPressedEvent>(&test_event_function);

			//AABB a,b;
			//Vector2 vel1 = { 0,0 };
			//Vector2 vel2 = { 0,0 };
			//a.min = { 1,2 };
			//a.max = { 3,4 };
			//b.min = { 4,4 };
			//b.max = { 5,6 };
			//if (CollisionIntersection_RectRect(a, vel1, b, vel2))
			//{
			//	std::cout << "colliding\n";
			//}
			//else
			//{
			//	std::cout << "not colliding\n";
			//}


		}
		RenderModule::openGLSystem->Exit();
		delete RenderModule::openGLSystem;
	}
}
