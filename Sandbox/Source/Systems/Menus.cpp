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
#include "Menus.h"


#include <rttr/registration.h>

#include "Audio/AudioEngine.h"
#include "EnemySpawner/EnemySpawner.h"
#include "Systems/UISystem.h"

namespace DeltaEngine
{


void ButtonHover()
{
  AudioEngine::Play2DEvent( "event:/UI/ButtonMouseOver4" );
}

void Back()
{
  UISystem::PopScreen();
}

void StartGame()
{
  GetEnv().pECS->GetWorld().GetEntityManager().Clear();
  GetEnv().pECS->GetWorld().Load( "World/gam250.json" );
}

void OpenControls()
{
  UISystem::PushScreen( 2 );
}

void OpenOptions()
{
  UISystem::PushScreen( 3 );
}

void OpenCredits()
{
  UISystem::PushScreen( 4 );
}

/*
 * QUIT GAME MAIN MENU
 */

void QuitGame()
{
  UISystem::PushScreen( 5 );
}

void ConfirmQuitGame()
{
  env.pECS->GetWorld().FindOrCreateSystem<EnemySpawner>().Shutdown();
  env.pWin->Running( false );
}


/*
* QUIT GAME IN-GAME
*/

void QuitInGame()
{
  UISystem::PushScreen( 6 );
}

void ConfirmQuitInGame()
{
  UISystem::PopScreen();
  GetEnv().pECS->GetWorld().GetEntityManager().Clear();
  GetEnv().pECS->GetWorld().Load( "World/MainMenuScreen.json" );
}

/*
* Pause Game
*/
void PauseGame()
{
  UISystem::PushScreen( 1 );
}

RTTR_REGISTRATION
{
rttr::registration::method( "ButtonHover", &ButtonHover );
rttr::registration::method( "Back", &Back );
rttr::registration::method( "StartGame", &StartGame );
rttr::registration::method( "OpenControls", &OpenControls );
rttr::registration::method( "OpenOptions", &OpenOptions );
rttr::registration::method( "OpenCredits", &OpenCredits );
rttr::registration::method( "QuitGame", &QuitGame );
rttr::registration::method( "ConfirmQuitGame", &ConfirmQuitGame );
rttr::registration::method( "QuitInGame", &QuitInGame );
rttr::registration::method( "ConfirmQuitInGame", &ConfirmQuitInGame );
rttr::registration::method( "PauseGame", &PauseGame );
}
}