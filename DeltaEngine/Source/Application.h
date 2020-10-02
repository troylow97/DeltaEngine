#pragma once
#include "Core/LayerStack.h"
#include "Event/ApplicationEvent.h"
#include "DE_API.h"

namespace DeltaEngine
{
  class ImGuiLayer;

  class DE_API Application
	{

		bool m_Running;
		bool m_Minimized;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
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
