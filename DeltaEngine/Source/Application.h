#pragma once
#include "Core/LayerStack.h"
#include "Event/ApplicationEvent.h"
#include "DE_API.h"
#include "Core/GameClock/GameClock.h"
#include "Core/Debugging/Logger/Log.h"
#include "OECS/ECSModule.h"
#include <memory>
namespace DeltaEngine
{

	extern std::unique_ptr<ECSModule> ecs;


	class DE_API Application
	{
		bool m_Running;
		bool m_Minimized;
		LayerStack m_LayerStack;
		GameClock m_gameclock;
		double m_interval;

	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent();
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		void FixedUpdate();
		void VariableUpdate();
	};

	Application* CreateApplication();
}