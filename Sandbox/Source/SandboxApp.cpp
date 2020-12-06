#include <DeltaEngine.h>
#include "EntryPoint.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <chrono>
#include "../Source/Systems/AI/AI_StateMachine.h"
#include "Physics/CollisionSystem.h"
#include "Systems/AttackSystem.h"
#include "Systems/GCameraSystem.h"
#include "Systems/LifespanSystem.h"
#include "Systems/RespawnSystem.h"
#include "Systems/UpgradeSystem.h"
#include "Systems/UISystem.h"
#include "Systems/CollisionHandler/CollisionHandlingFunctions.h"
#include "Systems/EnemySpawner/EnemySpawner.h"

class Sandbox : public Application
{
public:
  Sandbox()
  {
      JsonFile file;
      env.pECS->GetWorld().Load("World/MainMenu.json");
  	
    CollisionSystem::collision_handler.RegisterOnStay(CollisionHandlerFunctions::TakeDamage);
    env.pECS->GetWorld().CreateSystems<AttackSystem, EnemySpawner, LifespanSystem, RespawnSystem, GCameraSystem, UISystem, UpgradeSystem>();
    env.pECS->GetWorld().SetUpdateSequence<AttackSystem, EnemySpawner, LifespanSystem, RespawnSystem, UpgradeSystem>();
    env.pECS->GetWorld().SetLateUpdateSequence<GCameraSystem, UISystem>();
    env.pECS->GetWorld().InitSystems();
    CollisionHandlerFunctions::Initialise();
  }

  ~Sandbox()
  {
  }
};

Application* DeltaEngine::CreateApplication()
{
  return new Sandbox();
}
