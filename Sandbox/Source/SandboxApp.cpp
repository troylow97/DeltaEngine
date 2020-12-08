/**********************************************************************************
* \file   Sandbox.cpp
* \brief  The file contains BLAHBLAHBLAH
* \author Chin, Clara,   X% Code Contribution
* \author Low, Troy,     X% Code Contribution
* \author Ong, Graeme,   X% Code Contribution
* \author Tan, Tong Wee, X% Code Contribution
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
#include "Systems/AI/ExitScene/ExitSceneCinematic.h"


class Sandbox : public Application
{
public:
  Sandbox()
  {
    JsonFile file;
    //env.pECS->GetWorld().Load("World/MainLevelV2.json");
    env.pECS->GetWorld().Load("World/MainMenu.json");

    CollisionSystem::collision_handler.RegisterOnStay(CollisionHandlerFunctions::TakeDamage);
    env.pECS->GetWorld().CreateSystems<AttackSystem, EnemySpawner, LifespanSystem, RespawnSystem, GCameraSystem, UISystem, UpgradeSystem,ExitSceneCinematic>();
    env.pECS->GetWorld().SetUpdateSequence< AttackSystem, EnemySpawner, LifespanSystem, RespawnSystem, UpgradeSystem, ExitSceneCinematic >();
    env.pECS->GetWorld().SetLateUpdateSequence<GCameraSystem, UISystem>();
    env.pECS->GetWorld().InitSystems();
    CollisionHandlerFunctions::Initialise();

    //AudioEngine::Play("Audio/MainMenu/main_menu_bgm.wav");

  }

  ~Sandbox()
  {
  }
};

Application* DeltaEngine::CreateApplication()
{
  return new Sandbox();
}
