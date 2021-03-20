/**********************************************************************************
* \file   UISystem.cpp
* \brief  The file contains the system for updating and displaying UI in the game
* \author Chin, Clara,     70% Code Contribution
* \author Low, Troy,       30% Code Contribution
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
#include "Input/InputManager.h"
#include "Input/Keys.h"

namespace DeltaEngine
{

GameState current {GameState::NONE};

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
      world.Load( "World/MainMenuScreen.json" );
      current = GameState::MAIN_MENU;
      break;
    }
    case GameState::TUTORIAL :
    {
      world.Load( "World/gam250tutorial.json" );
      current = GameState::TUTORIAL;
      break;
    }
    case GameState::LEVEL_1 :
    {

      current = GameState::LEVEL_1;
      break;
    }
  }
}

GameState GameStateCurrent()
{
  return current;
}

}