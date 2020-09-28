#include <DeltaEngine.h>
#include "EntryPoint.h"

class Sandbox : public DeltaEngine::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}

};

DeltaEngine::Application* DeltaEngine::CreateApplication()
{
	return new Sandbox();
}

