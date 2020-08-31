#pragma once
#include "Core.h"
#include "Core/InputSystem.h"
//#include "Input.h"

namespace DeltaEngine
{
	class DE_API Application
	{

	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* CreateApplication();
}


