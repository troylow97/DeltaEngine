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
#include "Systems/GCameraSystem.h"
#include "Systems/LifespanSystem.h"
#include "Systems/RespawnSystem.h"
#include "Systems/UpgradeSystem.h"
#include "Systems/UISystem.h"
#include "Systems/CollisionHandler/CollisionHandlingFunctions.h"
#include "Systems/EnemySpawner/EnemySpawner.h"
#include "Audio/AudioEngine.h"
#include "Systems/HealthSystem.h"
#include "Systems/AI/ExitScene/ExitSceneCinematic.h"


class Sandbox : public Application
{
public:
  Sandbox()
  {

#ifndef DE_EDITOR
    AudioEngine::SetEventVolume(AudioEngine::Play2DEvent("event:/BGM/BGM1"),-12.0f);
    JsonFile file;
    env.pECS->GetWorld().Load("World/gam250alphaWithCollider.json");
#endif


    CollisionSystem::collision_handler.RegisterOnStay(CollisionHandlerFunctions::TakeDamage);
    env.pECS->GetWorld().CreateSystems<AttackSystem, /*EnemySpawner*/ LifespanSystem, RespawnSystem, GCameraSystem,
                                       UISystem/*, UpgradeSystem, ExitSceneCinematic*/, HealthSystem>();
    env.pECS->GetWorld().SetUpdateSequence<AttackSystem, /*EnemySpawner*/ HealthSystem, RespawnSystem, LifespanSystem>();
    //env.pECS->GetWorld().SetUpdateSequence<AttackSystem, EnemySpawner, HealthSystem, RespawnSystem, UpgradeSystem,
    //    ExitSceneCinematic, LifespanSystem>();
    env.pECS->GetWorld().SetLateUpdateSequence<GCameraSystem/*, UISystem*/>();
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
