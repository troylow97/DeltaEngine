/**********************************************************************************
* \file   Application.cpp
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
#include "DeltaEngine.h"
#include "Application.h"

#include "EngineConfig.h"
#include "Assets/Loaders/AudioLoader.h"
#include "Render/OpenGLSystem.h"
#include "Core/Utils/FileUtils.h"
#include "Core/Utils/DirectoryWatcher/SystemDirectory.h"
#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"
#include "Physics/CollisionSystem.h"
#include "ECS/World.h"
#include "Input/InputManager.h"
#include "Audio/AudioEngine.h"
#include "ImGui/Editor.h"
#include "ImGui/Panels/LoggerPanel.h"
#include "Core/Debugging/Profiler/Profiler.h"
#include "Core/Utils/Random.h"

/*-----------------------------------
#include "Event/ApplicationEvent.h"
#include "Log.h"
-----------------------------------*/
#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif
namespace DeltaEngine
{
DeltaEngineGlobalEnvironment env;

Application::Application()
{
  // Logger Initialization
  Log::Init();
  DeltaEngine_CORE_INFO( "Engine Start" );

  // Randomizer
  Random::Init();

  // Filesystem Initialization
  DeltaEngine_CORE_INFO( "Root Directory - Assets" );
  FileUtils::Root( "Assets" );
  SystemDirectory::Instance().Initialize();

  // Engine Initialization
  JsonFile f;
  EngineConfig c;
  f.StartReader( "config.json" ).LoadObject( c ).EndReader();

  // Audio Initialization
  AudioEngine::Initialize();
  AudioLoader().Load();

  // Clock Initialization
  env.pClock = new EngineClock( c.fps );

  // Window Initialization
  env.pWin = new Window( c.win_name, c.width, c.height, c.fullscreen );
  env.pWin->Init();

  // Render Initialization
  RenderModule::openGLSystem = new RenderModule::OpenGLSystem();
  RenderModule::openGLSystem->Init();

  // Asset Manager Initialization and Loading
  env.pManager = new AM();
  DeltaEngine_CORE_INFO( "Initializing AssetManager..." );
  DeltaEngine_CORE_INFO( "Asset Manager Setting FontLoader with fallback, Fonts/Arial.ttf" );
  env.pManager->SetLoader<Font>( new FontLoader() ).Load<Font>()
    .SetFallback<Font>( new Font( "Fonts/Arial.ttf" ) );

  DeltaEngine_CORE_INFO( "AssetManager Setting ShaderLoader with fallback, Shaders/ErrorShader" );
  env.pManager->SetLoader<Shader>( new ShaderLoader() ).Load<Shader>()
    .SetFallback<Shader>( new Shader( "Shaders/ErrorShader" ) );

  DeltaEngine_CORE_INFO( "AssetManager Setting TextureLoader with no fallback" );
  env.pManager->SetLoader<Texture2D>( new TextureLoader() ).Load<Texture2D>()
    .SetFallback<Texture2D>( new Texture2D( "Default/ERROR.png" ) );

  DeltaEngine_CORE_INFO( "AssetManager Setting AnimationClipLoader with no fallback" );
  env.pManager->SetLoader<AnimationClip>( new AnimationClipLoader() ).Load<AnimationClip>();

  DeltaEngine_CORE_INFO( "AssetManager Setting AnimationControllerLoader with no fallback" );
  env.pManager->SetLoader<AnimationController>( new AnimationControllerLoader() ).Load<AnimationController>();
  DeltaEngine_CORE_INFO( "Initializing AssetManager successful" );

   // Event Manager Initialization
  env.eventManager = new EventManager;

  // ECS Initialization
  env.pECS = new ECSModule();
#ifdef DE_EDITOR
  //auto id = env.pECS->GetWorld().GetEntityManager().CreateEntity<Camera>();
  //env.pECS->GetWorld().GetEntityManager().GetComponent<EntityName>( id).name.assign( "Camera");
  //env.pECS->GetWorld().GetEntityManager().GetComponent<Camera>( id ).m_Size = c.cam_size;
  Editor::Instance();
  SystemDirectory::Instance().StartWatch();
#endif
}

Application::~Application()
{
  DeltaEngine_CORE_INFO( "Engine Shutdown" );

#ifdef DE_EDITOR
  SystemDirectory::Instance().StopWatch();
#endif
  delete env.pECS;
  delete env.eventManager;
  DeltaEngine_CORE_INFO( "Shutting down AssetManager..." );
  delete env.pManager;
  DeltaEngine_CORE_INFO( "Shutting down AssetManager successful" );
  RenderModule::openGLSystem->Exit();
  delete RenderModule::openGLSystem;
  env.pWin->Shutdown();
  delete env.pWin;
  DeltaEngine_CORE_INFO( "Shutting down Gameclock..." );
  delete env.pClock;
  DeltaEngine_CORE_INFO( "Shutting down Gameclock successful" );

  AudioEngine::Shutdown();
  SystemDirectory::Instance().Shutdown();
}

void Application::Run()
{
  while ( env.pWin->Running() )
  {
    if ( env.pWin->Focus() )
    {
      Profiler::Instance().FrameStart();
      env.pClock->Update();
      InputManager::Instance().Update();
      env.pECS->GetWorld().Run();
#ifdef DE_EDITOR
      Editor::Instance().Begin();
      Editor::Instance().Render();
      Editor::Instance().End();
      Profiler::Instance().Record( "ImGui" );
#endif
      SwapBuffers( RenderModule::openGLSystem->GetWindowContext() );
      Profiler::Instance().Record( "Buffer Swap" );
      OnEvent();
      env.pWin->Update();
      AudioEngine::Update();
      Profiler::Instance().FrameEnd();
    }
    else
    {
#ifdef DE_EDITOR
      Editor::Instance().Begin();
      Editor::Instance().Render();
      Editor::Instance().End();
#endif
      env.pWin->Update();
    }
  }
}

void Application::OnEvent()
{
  if ( !env.eventManager->IsEmpty() )
  {
    auto ref = env.eventManager->ResolveEvent();
    EventDispatcher d( ref );

    if ( ref != nullptr )
    {
      EventType type = ref->GetEventType();
      switch ( type )
      {
        case EventType::ImGuiDragFile:
        {
          d.Dispatch<ImGuiFileDragEvent>( DE_BIND_EVENT_FN( Editor::OnDragDrop ) );
          break;
        }
        case EventType::ImGuiRemovingDragFile:
        {
          d.Dispatch<ImGuiFileRemovingDragEvent>( DE_BIND_EVENT_FN( Editor::OnRemovingDragDrop ) );
          break;
        }
        case EventType::ImGuiFileDragDone:
        {
          d.Dispatch<ImGuiFileDragEventDone>( DE_BIND_EVENT_FN( Editor::OnDragDropDone ) );
          break;
        }
      }
    }
    delete ref;
  }
  Profiler::Instance().Record( "Event" );
}

}