#include <DeltaEngine.h>
#include "EntryPoint.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <chrono>
#include "../Source/Systems/AI/AI_StateMachine.h"
#include "Physics/CollisionSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Systems/AnimationSystem.h"
#include "Systems/InputSystem.h"
#include "Systems/PhysicsDrawSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/AttackSystem.h"
#include "Systems/LifespanSystem.h"
#include "Systems/RespawnSystem.h"
#include "Systems/AI/AI_StateMachine.h"
#include "Systems/CollisionHandler/CollisionHandlingFunctions.h"
#include "Systems/EnemySpawner/EnemySpawner.h"

class Sandbox : public Application
{
public:
  Sandbox()
  {
    CollisionSystem::collision_handler.RegisterOnStay(TakeDamage);
    env.pECS->GetWorld().CreateSystems<AttackSystem, EnemySpawner,LifespanSystem, RespawnSystem>();
    env.pECS->GetWorld().SetUpdateSequence<AttackSystem, EnemySpawner,LifespanSystem, RespawnSystem >();

    env.pECS->GetWorld().InitSystems();
  }

  ~Sandbox()
  {
  }
};

Application* DeltaEngine::CreateApplication()
{
  return new Sandbox();
}
