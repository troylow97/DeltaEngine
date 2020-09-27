#include "DEpch.h"
#include "Application.h"
#include "../Render/GraphicsManager.h"
#include "../Render/SpriteRenderer.h"
#include "Event/MouseEvent.h"
#include "Event/ApplicationEvent.h"
#include "Physics/CollisionManager.h"
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

		Camera* mainCamera = new Camera();
		SpriteRenderer* a = new SpriteRenderer;
		sprites.push_back(a);
		a->transform.position.x = 30;
		a->transform.position.y = 30;
		a->transform.scale.x = 5;
		a->transform.scale.y = 5;
		a->collider._aabb.min.x = a->transform.position.x - a->transform.scale.x / 2;
		a->collider._aabb.min.y = a->transform.position.y - a->transform.scale.y / 2;
		a->collider._aabb.max.x = a->transform.position.x + a->transform.scale.x / 2;
		a->collider._aabb.max.y = a->transform.position.y + a->transform.scale.y / 2;
		CollisionManager collisionManager;
		collisionManager.GameObjects.push_back(a);
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
			collisionManager.CollisionCheck();
			//KeyPressedEvent keyevent{3,69};
			//EventDispatcher dispatcher(e);

			RenderModule::openGLSystem->TestRender(sprites);
			//Input::InputSystem input_system;
			

			//if (keyevent.isHandled)
			//	break;

			//dispatcher.Dispatch<KeyPressedEvent>(&test_event_function);

		}
		RenderModule::openGLSystem->Exit();
		delete RenderModule::openGLSystem;
	}
}
