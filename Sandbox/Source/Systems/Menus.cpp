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
#include "../GameState.h"

#include <rttr/registration.h>

#include "Audio/AudioEngine.h"
#include "EnemySpawner/EnemySpawner.h"
#include "Systems/UISystem.h"
#include "RespawnSystem.h"
#include "Input/InputManager.h"
#include "Input/Keys.h"
#include "LevelManager/LevelManager.h"

namespace DeltaEngine
{
float MenuSystem::fadeTimer = 0;
bool MenuSystem::fading = false;

void MenuSystem::Update()
{
  if ( InputManager::Instance().IsKeyReleased( DEVK_ESCAPE ) && !InputManager::Instance().IsKeyPressed(DEVK_ESCAPE) )
  {
    if ( GameStateCurrent() != GameState::MAIN_MENU )
    {
      if ( !UISystem::CurrentScreen() )
      {
        UISystem::PushScreen( 1 );
        GetEnv().pClock->TimeScale( 0.0f );
      }
      else
      {
        UISystem::PopScreen();

        if ( !UISystem::CurrentScreen() )
          GetEnv().pClock->TimeScale( 1.0f );
      }
      AudioEngine::SetEventVolume(AudioEngine::Play2DEvent( "event:/UI Sounds/Button Click" ), 0.4f);

    }
    else if ( UISystem::CurrentScreen() )
    {
      AudioEngine::SetEventVolume(AudioEngine::Play2DEvent( "event:/UI Sounds/Button Click" ), 0.4f);
      UISystem::PopScreen();
    }
  }
}

void MenuSystem::LateUpdate()
{

}

void ButtonHover()
{
  AudioEngine::SetEventVolume( AudioEngine::Play2DEvent( "event:/UI Sounds/Button Mouse Over 4") , 0.5f );

}

void Back()
{
  UISystem::PopScreen();
}

void StartGame()
{
  MenuSystem::fading = true;
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
  GameStateLoad( GameState::MAIN_MENU );
}

/*
* In-Game Menu
*/
void Resume()
{
  UISystem::PopScreen();
  GetEnv().pClock->TimeScale( 1.0f );
}

void Restart()
{
  GameStateLoad( GameStateCurrent() );
}

void UpdateVolume(float volume)
{
  AudioEngine::SetMasterVolume( volume );
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
rttr::registration::method( "Resume", &Resume );
rttr::registration::method( "Restart", &Restart );
rttr::registration::method( "MasterVolumeControl", &UpdateVolume );
}
}
