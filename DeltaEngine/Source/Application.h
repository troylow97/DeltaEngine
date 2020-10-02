#pragma once
#include "Core/LayerStack.h"
#include "Event/ApplicationEvent.h"
#include "DE_API.h"
#include "Core/GameClock/GameClock.h"
#include "Core/Debugging/Logger/Log.h"

namespace DeltaEngine
{
	class DE_API Application
	{

		bool m_Running;
		bool m_Minimized;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
		GameClock m_gameclock;
		double m_interval;
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);
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