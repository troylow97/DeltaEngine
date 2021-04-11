/**********************************************************************************
* \file   LevelManageer.cpp
* \brief  The file contains the system for spawning enemies in the game
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "LevelManager.h"

#include "../RespawnSystem.h"
#include "../UnitManager.h"
#include "../AI/AITools.h"
#include "../EnemySpawner/EnemySpawner.h"
#include "../../GameState.h"
#include "../Menus.h"
#include "Render/Cutscenes.h"
#include "Input/InputManager.h"
#include "Input/Keys.h"

namespace DeltaEngine
{
void LevelManager::Update()
{
  switch(GameStateCurrent())
  {
  	case GameState::MAIN_MENU:
  	{
  	  if (MenuSystem::fading)
  	  {
  	  	MenuSystem::fadeTimer += DeltaTimef() / 2;
  	  	Camera::allCameras[0]->backgroundColor.a = 1 - MenuSystem::fadeTimer;
  	  	if (MenuSystem::fadeTimer > 5 / 2)
  	  	{
  	  	  MenuSystem::fadeTimer = 0;
  	  	  MenuSystem::fading = false;
  	  	  GameStateLoad(GameState::CUTSCENE_INTRO);
  	  	}
  	  }
  	  break;
  	}
  	case GameState::CUTSCENE_INTRO:
  	{
  	  Cutscene::timer += DeltaTimef() * 2;
  	  if (Cutscene::timer / 2 > Cutscene::introTimings[Cutscene::frame + 1])
  	  {
  	  	++Cutscene::frame;
  	  	Cutscene::timer = 0;
  	  }
  	  if (InputManager::Instance().IsKeyPressed(DEVK_SPACE) || 
  	  	Cutscene::frame > 9 && Cutscene::timer > 1)
  	  {
  	  	GameStateLoad(GameState::TUTORIAL);
  	  }
  	  break;
  	}
  	case GameState::TUTORIAL:
  	{
  	  if (MenuSystem::fading)
  	  {
  	  	MenuSystem::fadeTimer += DeltaTimef() / 2;
  	  	Camera::allCameras[0]->transitionTexKey = "Textures/Sharp_swipe_0";
  	  	Camera::allCameras[0]->backgroundColor.a = MenuSystem::fadeTimer;
  	  	if (MenuSystem::fadeTimer > 5 / 2)
  	  	{
  	  	  MenuSystem::fadeTimer = 0;
  	  	  MenuSystem::fading = false;
  	  	}
  	  }
      
  	  auto player = UnitManager::GetPlayerID();
  	  Vector2 EndPoint{ 290.0f,0.2f };
  	  if(AITools::EntityisAtPoint(player, EndPoint,0.2f))
  	  	GameStateLoad(GameState::LEVEL_1);
  	  break;
  	}
  	case GameState::LEVEL_1:
  	{
  	  if (MenuSystem::fading)
  	  {
  	  	MenuSystem::fadeTimer += DeltaTimef() / 2;
  	  	Camera::allCameras[0]->transitionTexKey = "Textures/Sharp_swipe_0";
  	  	Camera::allCameras[0]->backgroundColor.a = MenuSystem::fadeTimer;
  	  	if (MenuSystem::fadeTimer > 5 / 2)
  	  	{
  	  	  MenuSystem::fadeTimer = 0;
  	  	  MenuSystem::fading = false;
  	  	}
  	  }
  	  break;
  	}
  	default:
  	{
  	  break;
  	}
  }
}
void LevelManager::LateUpdate()
{
}
}

