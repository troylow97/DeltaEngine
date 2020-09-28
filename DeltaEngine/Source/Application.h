#pragma once
#include "DE_API.h"

namespace DeltaEngine
{
	class DE_API Application
	{

		double m_interval;
	public:
		Application();
		virtual ~Application();

		void Run();

	private:
		void FixedUpdate();
		void VariableUpdate();
	};

	Application* CreateApplication();
}