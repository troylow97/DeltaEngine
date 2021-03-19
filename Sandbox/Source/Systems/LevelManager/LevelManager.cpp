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
#include "Audio/AudioEngine.h"
#include "Core/GlobalStruct.h"
#include "Systems/UISystem.h"

namespace DeltaEngine
{
	LevelType LevelManager::level;
	
	void LevelManager::Update()
	{
		switch(level)
		{
			case LevelType::MainMenu:
			{
				break;
			}
			case LevelType::Tutorial:
			{
				auto player = UnitManager::GetPlayerID();
				Vector2 EndPoint{ 290,0.2 };
				if(AITools::EntityisAtPoint(player, EndPoint,0.2f))
				{
					Init_Level1();
				}
				break;
			}
			case LevelType::Level1:
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
	
	void LevelManager::Init_MainMenu()
	{
		level = LevelType::MainMenu;
	}
	
	void LevelManager::Init_Tutorial()
	{
		level = LevelType::Tutorial;
	}

	void LevelManager::Init_Level1()
	{
		GetEnv().pECS->GetWorld().GetEntityManager().Clear();
		level = LevelType::Level1;
		GetEnv().pECS->GetWorld().Load("World/gam250beta_t.json");
		RespawnSystem::CreateCheckpoints(1);
		EnemySpawner::ActivateGauntlet = true;
		UISystem::ClearScreens();
		GetEnv().pClock->TimeScale(1.0f);
		AudioEngine::StopAllAudio();
	}

}

