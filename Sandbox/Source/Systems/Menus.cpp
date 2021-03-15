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
#include "Input/InputManager.h"
#include "Input/Keys.h"

namespace DeltaEngine
{
bool in_game { false };

void MenuSystem::Update()
{
  if ( InputManager::Instance().IsKeyReleased( DEVK_ESCAPE ) && !InputManager::Instance().IsKeyPressed(DEVK_ESCAPE) )
  {
    if ( in_game )
    {

      if ( !UISystem::CurrentScreen() )
      {
        AudioEngine::SetEventVolume(AudioEngine::Play2DEvent( "event:/UI Sounds/Button Click" ), 0.4f);
        UISystem::PushScreen( 1 );
        GetEnv().pClock->TimeScale( 0.0f );

      }
      else
      {
        UISystem::PopScreen();

        if ( !UISystem::CurrentScreen() )
        {
          AudioEngine::SetEventVolume(AudioEngine::Play2DEvent( "event:/UI Sounds/Button Click" ), 0.4f);
          GetEnv().pClock->TimeScale( 1.0f );
        }
      }
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
  AudioEngine::SetEventVolume(AudioEngine::Play2DEvent( "event:/UI Sounds/Button Click" ), 0.4f);
  UISystem::PopScreen();
}

void StartGame()
{
  AudioEngine::SetEventVolume(AudioEngine::Play2DEvent( "event:/UI Sounds/Button Click" ), 0.4f);
  GetEnv().pECS->GetWorld().GetEntityManager().Clear();
  GetEnv().pECS->GetWorld().Load( "World/gam250beta_t.json" );
  GetEnv().pClock->TimeScale( 1.0f );
  in_game = true;
  AudioEngine::StopAllAudio();
}

void OpenControls()
{
  AudioEngine::SetEventVolume(AudioEngine::Play2DEvent( "event:/UI Sounds/Button Click" ), 0.4f);
  UISystem::PushScreen( 2 );
}

void OpenOptions()
{
  AudioEngine::SetEventVolume(AudioEngine::Play2DEvent( "event:/UI Sounds/Button Click" ), 0.4f);
  UISystem::PushScreen( 3 );
}

void OpenCredits()
{
  AudioEngine::SetEventVolume(AudioEngine::Play2DEvent( "event:/UI Sounds/Button Click" ), 0.4f);
  UISystem::PushScreen( 4 );
}

/*
 * QUIT GAME MAIN MENU
 */

void QuitGame()
{
  AudioEngine::SetEventVolume(AudioEngine::Play2DEvent( "event:/UI Sounds/Button Click" ), 0.4f);
  UISystem::PushScreen( 5 );
}

void ConfirmQuitGame()
{
  AudioEngine::SetEventVolume(AudioEngine::Play2DEvent( "event:/UI Sounds/Button Click" ), 0.4f);
  env.pECS->GetWorld().FindOrCreateSystem<EnemySpawner>().Shutdown();
  env.pWin->Running( false );
}


/*
* QUIT GAME IN-GAME
*/

void QuitInGame()
{
  AudioEngine::SetEventVolume(AudioEngine::Play2DEvent( "event:/UI Sounds/Button Click" ), 0.4f);
  UISystem::PushScreen( 6 );
}

void ConfirmQuitInGame()
{
  AudioEngine::SetEventVolume(AudioEngine::Play2DEvent( "event:/UI Sounds/Button Click" ), 0.4f);
  UISystem::ClearScreens();
  GetEnv().pECS->GetWorld().GetEntityManager().Clear();
  GetEnv().pECS->GetWorld().Load( "World/MainMenuScreen.json" );
  GetEnv().pClock->TimeScale( 1.0f );
  in_game = false;
  AudioEngine::StopAllAudio();
}

/*
* In-Game Menu
*/
void Resume()
{
  AudioEngine::SetEventVolume(AudioEngine::Play2DEvent( "event:/UI Sounds/Button Click" ), 0.4f);
  UISystem::PopScreen();
  GetEnv().pClock->TimeScale( 1.0f );
}

void Restart1()
{
  AudioEngine::SetEventVolume(AudioEngine::Play2DEvent( "event:/UI Sounds/Button Click" ), 0.4f);
  UISystem::ClearScreens();
  GetEnv().pECS->GetWorld().GetEntityManager().Clear();
  GetEnv().pECS->GetWorld().Load( "World/gam250beta_t.json" );
  GetEnv().pClock->TimeScale( 1.0f );
  in_game = true;
  AudioEngine::StopAllAudio();

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
rttr::registration::method( "Restart1", &Restart1 );

}
}