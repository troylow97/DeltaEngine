#pragma once
#include "Core/LayerStack.h"
#include "Event/ApplicationEvent.h"
#include "DE_API.h"
#include "Core/GameClock/GameClock.h"
#include "Core/Debugging/Logger/Log.h"
#include "ImGui/ImGuiLayer.h"
#include "OECS/ECSModule.h"
#include <memory>
#include "Physics/PhysicsSystem.h"

namespace DeltaEngine
{
	extern std::unique_ptr<ECSModule> ecs;


	class DE_API Application
	{

		bool m_Running;
		bool m_Minimized;
		class PhysicsSystem* m_PhysicsSystem;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;
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
