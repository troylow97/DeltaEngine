#include "DeltaEngine.h"
#include "Application.h"

#include "EngineConfig.h"
#include "Render/OpenGLSystem.h"
#include "Core/Utils/FileUtils.h"
#include "Core/Utils/DirectoryWatcher/DirectoryWatcher.h"
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
int fps;

Application::Application() : m_Minimized { true }, m_interval( 0.25 )
{
  // Logger Initialization
  Log::Init();
  DeltaEngine_CORE_INFO( "Engine Start" );

  // Randomizer
  Random::Init();

  // Filesystem Initialization
  FileUtils::Root( "Assets" );
  SystemDirectory::Instance().Initialize();

  // Engine Initialization
  JsonFile f;
  EngineConfig c;
  f.StartReader( "config.json" ).LoadObject( c ).EndReader();

  // Audio Initialization
  AudioEngine::Initialize();

  // Clock Initialization
  env.pClock = new GameClock( c.fps );

  // Window Initialization
  env.pWin = new Window( c.win_name, c.width, c.height, c.fullscreen );
  env.pWin->Init();

  // Render Initialization
  RenderModule::openGLSystem = new RenderModule::OpenGLSystem();
  RenderModule::openGLSystem->Init();

  // Asset Manager Initialization and Loading
  env.pManager = new AM();
  env.pManager->SetLoader<Font>( new FontLoader() ).Load<Font>()
    .SetFallback<Font>( new Font( "Fonts/Arial.ttf" ) );

  env.pManager->SetLoader<Shader>( new ShaderLoader() ).Load<Shader>()
    .SetFallback<Shader>( new Shader( "Shaders/ErrorShader" ) );

  env.pManager->SetLoader<Texture2D>( new TextureLoader() ).Load<Texture2D>();

  env.pManager->SetLoader<AnimationClip>( new AnimationClipLoader() ).Load<AnimationClip>();

  env.pManager->SetLoader<AnimationController>( new AnimationControllerLoader() ).Load<AnimationController>();

//  // Editor Initialization
#ifdef DE_EDITOR
  m_Editor = new Editor();
#endif

    // Event Manager Initialization
  env.eventManager = new EventManager;

  // ECS Initialization
  env.pECS = new ECSModule();

 #ifdef DE_EDITOR
  SystemDirectory::Instance().StartWatch();
#endif
}

Application::~Application()
{
  DeltaEngine_CORE_INFO( "Engine Shutdown" );

#ifdef DE_EDITOR
  SystemDirectory::Instance().StopWatch();
#endif
  env.pECS->GetWorld().ShutdownSystems();
  delete env.pECS;
  delete env.eventManager;
#ifdef DE_EDITOR
  delete m_Editor;
#endif
  delete env.pManager;
  delete Camera::editorCamera;
  RenderModule::openGLSystem->Exit();
  delete RenderModule::openGLSystem;
  delete env.pWin;
  delete env.pClock;

  AudioEngine::Shutdown();
  SystemDirectory::Instance().Shutdown();
}


void Application::Run()
{
  auto entitycamera = env.pECS->GetWorld().GetEntityManager().CreateEntity<Transform, Camera>();

  while ( env.pWin->Running() )
  {
    if ( env.pWin->Focus() )
    {
      Profiler::Instance().FrameStart();
      env.pClock->Update();
      InputManager::Instance().Update();
      env.pECS->GetWorld().Update();
#ifdef DE_EDITOR
      m_Editor->Begin();
      m_Editor->Render();
      m_Editor->End();
#endif
      SwapBuffers( RenderModule::openGLSystem->GetWindowContext() );
      Profiler::Instance().Record( "Buffer Swap" );
      OnEvent();
      env.pWin->Update();
      Profiler::Instance().FrameEnd();
    }
    else
      env.pWin->Update();
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


bool Application::OnWindowResize( WindowResizeEvent &e )
{
  if ( e.GetWidth() == 0 || e.GetHeight() == 0 )
  {
    m_Minimized = true;
    return false;
  }

  m_Minimized = false;

  return false;
}
}