#pragma once
#include "DE_API.h"
#include "Core/GameClock/GameClock.h"
#include "Core/Logger/Log.h"

namespace DeltaEngine
{
	class DE_API Application
	{

		GameClock m_gameclock;
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