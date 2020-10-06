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
		double m_interval;
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		void FixedUpdate();
		void VariableUpdate();
	};

	Application* CreateApplication();
}
