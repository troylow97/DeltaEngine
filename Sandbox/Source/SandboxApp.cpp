/**********************************************************************************
* \file   Sandbox.cpp
* \brief  The file contains code to run the main game
* \author Chin, Clara,   50% Code Contribution
* \author Low, Troy,     50% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include <DeltaEngine.h>
#include "EntryPoint.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <chrono>
#include "../Source/Systems/AI/AI_StateMachine.h"
#include "Physics/CollisionSystem.h"
#include "Systems/AttackSystem.h"
#include "Systems/VFXSystem.h"
#include "Systems/EnemySpawner/EnemySpawner.h"
#include "Systems/LifespanSystem.h"
#include "Systems/RespawnSystem.h"
#include "Systems/GCameraSystem.h"
#include "Systems/UpgradeSystem.h"
#include "Systems/HealthSystem.h"
#include "Systems/CollisionHandler/CollisionHandlingFunctions.h"
#include "Audio/AudioEngine.h"
#include "Systems/Menus.h"
#include "Systems/AudioSystem.h"

class Sandbox : public Application
{
public:
  Sandbox()
  {

#ifndef DE_EDITOR
    JsonFile file;
    env.pECS->GetWorld().Load("World/MainMenuScreen.json");
#endif


    CollisionSystem::collision_handler.RegisterOnEnter(CollisionHandlerFunctions::TakeDamage);
    CollisionSystem::collision_handler.RegisterOnStay(CollisionHandlerFunctions::CheckGroundType);
    env.pECS->GetWorld().CreateSystems<AttackSystem, VFXSystem, EnemySpawner, LifespanSystem, RespawnSystem, GCameraSystem, HealthSystem, MenuSystem,AudioSystem>();
    env.pECS->GetWorld().SetUpdateSequence<AttackSystem, VFXSystem, EnemySpawner, HealthSystem, RespawnSystem, LifespanSystem, MenuSystem>();
    //env.pECS->GetWorld().SetUpdateSequence<AttackSystem, EnemySpawner, HealthSystem, RespawnSystem, UpgradeSystem,
    //    ExitSceneCinematic, LifespanSystem>();
    env.pECS->GetWorld().SetLateUpdateSequence<GCameraSystem, AudioSystem>();
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
