#pragma once 
#include "Core.h"
#include "Core/InputListener.h"

namespace DeltaEngine
{
	class DE_API Application //: public Input::InputSystem
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* CreateApplication();
}


