#include <DeltaEngine.h>
#include "EntryPoint.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <chrono>
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