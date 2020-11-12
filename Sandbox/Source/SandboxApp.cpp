#include <DeltaEngine.h>
#include "EntryPoint.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <chrono>



#include "AI/AI_StateMachine.h"
#include "Physics/CollisionSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Systems/AnimationSystem.h"
#include "Systems/InputSystem.h"
#include "Systems/PhysicsDrawSystem.h"
#include "Systems/RenderSystem.h"

class Sandbox : public DeltaEngine::Application
{
public:
	Sandbox()
	{
		env.pECS->GetWorld().CreateSystems<InputSystem, AISystem, PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem, PhysicsDrawSystem>();
		env.pECS->GetWorld().SetUpdateSequence<InputSystem, AISystem, PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem, PhysicsDrawSystem>();
		env.pECS->GetWorld().SetLateUpdateSequence<PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem, PhysicsDrawSystem>();
		env.pECS->GetWorld().InitSystems();
	}

	~Sandbox()
	{

	}
};

DeltaEngine::Application* DeltaEngine::CreateApplication()
{
	return new Sandbox();
}