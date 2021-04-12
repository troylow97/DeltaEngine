/**********************************************************************************
* \file   GameState.cpp
* \brief  The file contains the implementation for GameState Management
* \author Tan Tong Wee     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "GameState.h"

#include "Audio/AudioEngine.h"
#include "Systems/EnemySpawner/EnemySpawner.h"
#include "Systems/UISystem.h"
#include "Systems/RespawnSystem.h"
#include "Systems/Menus.h"
#include "Input/InputManager.h"
#include "Input/Keys.h"
#include "Render/Cutscenes.h"

namespace DeltaEngine
{

GameState current {GameState::MAIN_MENU };

void GameStateLoad( GameState state )
{
  auto &world = GetEnv().pECS->GetWorld();
  auto &em = world.GetEntityManager();
  em.Clear();
  AudioEngine::StopAllAudio();
  UISystem::ClearScreens();
  GetEnv().pClock->TimeScale( 1.0f );

  switch ( state )
  {
    case GameState::MAIN_MENU :
    {
      current = GameState::MAIN_MENU;
      world.Load( "World/MainMenuScreen.json" );
      MenuSystem::fadingIn = true;
      Camera::allCameras[0]->backgroundColor.a = 0;
      EnemySpawner::ActivateGauntlet = false;
      break;
    }
    case GameState::CUTSCENE_INTRO :
    {
      current = GameState::CUTSCENE_INTRO;
      world.Load("World/IntroCutscene.json");
      Cutscene::timer = 0;
      Cutscene::frame = -1;
      Cutscene::totalFrames = 10;
      Cutscene::cutscenePrefix = "Textures/INTRO_";
      break;
    }
    case GameState::TUTORIAL :
    {
      current = GameState::TUTORIAL;
      world.Load( "World/gam250tutorial.json" );
      world.Load( "World/GameMenuScreen.json" );
      world.Load( "World/ps.json" );
      MenuSystem::fadingIn = true;
      Camera::allCameras[0]->backgroundColor.a = 0;
      RespawnSystem::DestroyCheckpoints();
      RespawnSystem::CreateCheckpoints(0);
      EnemySpawner::ActivateGauntlet = false;
      break;
    }
    case GameState::LEVEL_1 :
    {
      current = GameState::LEVEL_1;
      world.Load( "World/gam250beta_t.json" );
      world.Load( "World/GameMenuScreen.json" );
      world.Load( "World/ps.json" );
      MenuSystem::fadingIn = true;
      Camera::allCameras[0]->backgroundColor.a = 0;
      RespawnSystem::CreateCheckpoints(1);
      EnemySpawner::ActivateGauntlet = true;
      break;
    }
    case GameState::CUTSCENE_END:
    {
      current = GameState::CUTSCENE_END;
      world.Load("World/EndCutscene.json");
      Cutscene::timer = 0;
      Cutscene::frame = -1;
      Cutscene::totalFrames = 6;
      Cutscene::cutscenePrefix = "Textures/ENDING_";
      break;
    }
    case GameState::CREDITS:
    {
      current = GameState::CREDITS;
      world.Load("World/credits.json");
      MenuSystem::fadingIn = true;
      Camera::allCameras[0]->backgroundColor.a = 0;
      break;
    }
  }
}

GameState GameStateCurrent()
{
  return current;
}
}