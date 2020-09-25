#include "DEpch.h"
#include "Application.h"
#include "Render/GraphicsManager.h"
#include "Render/FrameAnimation.h"
#include "Render/ParticleSystem.h"
#include "Render/SpriteRenderer.h"
#include "Event/ApplicationEvent.h"
#include "Physics/Collision.h"

/*-----------------------------------
#include "Event/ApplicationEvent.h"
#include "Log.h"
-----------------------------------*/
namespace DeltaEngine
{
std::vector<SpriteRenderer *> sprites;
std::vector<FrameAnimation *> animator;
std::vector<ParticleSystem *> ps;

Application::Application() : m_interval( 0.25 )
{
  DeltaEngine::Log::Init();
  DeltaEngine_CORE_INFO( "Engine Start" );
  // Memory Manager
  // Window
  // Render
  // GUI
  // Physics
  // Audio
  // Events
}

Application::~Application()
{
  DeltaEngine_CORE_INFO( "Engine Shutdown" );
  // Events
  // Audio
  // Physics
  // GUI
  // Render
  // Window
  // Memory Manager
}

void Application::Run()
{
  // a lot of this should be moved to a function in GraphicsManager later

  RenderModule::openGLSystem = new RenderModule::OpenGLSystem();
  RenderModule::openGLSystem->Init();

  Camera *mainCamera = new Camera();

  sprites.push_back( new SpriteRenderer() );
  sprites.push_back( new SpriteRenderer() );
  animator.push_back( new FrameAnimation() );
  animator.push_back( new FrameAnimation() );
  animator[0]->renderer = sprites[0];
  animator[1]->renderer = sprites[1];

  ps.push_back( new ParticleSystem() );

  // TODO Modules Instantiation
  f64 accumulator = 0.0;

  MSG msg = {};
  while ( msg.message != WM_QUIT )
  {
    m_gameclock.Update(); // Update engine GameClock

    accumulator += m_gameclock.DeltaTime(); // Update accumulator using time-scaled dt

    while ( accumulator >= m_interval ) // Update based on interval
    {
      if ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
      {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
        continue;
      }
      animator[0]->Update();
      animator[1]->Update();
      RenderModule::openGLSystem->TestRender( sprites, ps );				
      FixedUpdate();
      accumulator -= m_gameclock.DeltaTime();
    }
    const f64 alpha = accumulator / m_interval;


  }
  RenderModule::openGLSystem->Exit();
  delete RenderModule::openGLSystem;
}

void Application::FixedUpdate()
{
  // Collision Update
  // Collision Resolution Update
  // Collision Late Update
  // Level Fixed Update
}

void Application::VariableUpdate()
{
  // Input Update
  // Level Update
  // Events Update (Logics)
  // Level Late Update
  // Audio Update
  // Render Update
  // GUI Update
  // Memory Update

}

}
