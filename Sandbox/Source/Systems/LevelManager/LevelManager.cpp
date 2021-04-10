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

namespace DeltaEngine
{
	
	void LevelManager::Update()
	{
		switch(GameStateCurrent())
		{
			case GameState::MAIN_MENU:
			{
				break;
			}
			case GameState::TUTORIAL:
			{
				auto player = UnitManager::GetPlayerID();
				Vector2 EndPoint{ 290.0f,0.2f };
				if(AITools::EntityisAtPoint(player, EndPoint,0.2f))
					GameStateLoad(GameState::LEVEL_1);
				break;
			}
			case GameState::LEVEL_1:
			{
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

