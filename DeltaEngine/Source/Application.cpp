#include "DeltaEngine.h"
#include "Application.h"

#include "EngineConfig.h"
#include "Render/OpenGLSystem.h"
#include "Core/Utils/FileUtils.h"
#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"
#include "Physics/PhysicsSystem.h"
#include "Physics/CollisionSystem.h"
#include "Systems/AnimationSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/PhysicsDrawSystem.h"
#include "Systems/InputSystem.h"
#include "ECS/World.h"
#include "Input/InputManager.h"
#include "Audio/AudioEngine.h"
#include "ImGui/Editor.h"
#include "AI/AI_StateMachine.h"
#include "Core/Utils/Random.h"
#include "Systems/AttackSystem.h"
/*-----------------------------------
#include "Event/ApplicationEvent.h"
#include "Log.h"
-----------------------------------*/

namespace DeltaEngine
{
DeltaEngineGlobalEnvironment env;
int fps;
Application::Application() : m_Minimized { true }, m_interval( 0.25 )
{
  Log::Init();
  DeltaEngine_CORE_INFO( "Engine Start" );
  FileUtils::Root( "Assets" );

  // Load Engine Configuration
  JsonFile f;
  EngineConfig c;
  f.StartReader( "config.json" ).LoadObject( c ).EndReader();
  AudioEngine::Initialize();
  env.pClock = new GameClock( c.fps );

  env.pWin = new Window( c.win_name, c.width, c.height );
  env.pWin->Init();

  // Render + Imgui
  RenderModule::openGLSystem = new RenderModule::OpenGLSystem();
  RenderModule::openGLSystem->Init();

  m_Editor = new Editor();

  Random::Init();

  // Asset Loading
  env.pManager = new AM();
  env.pManager->SetLoader<Font>( new FontLoader() ).Load<Font>()
    .SetFallback<Font>( new Font( "Fonts/Arial.ttf" ) );

  env.pManager->SetLoader<Shader>( new ShaderLoader() ).Load<Shader>()
    .SetFallback<Shader>( new Shader( "Shaders/ErrorShader" ) );

  env.pManager->SetLoader<Texture2D>( new TextureLoader() ).Load<Texture2D>();

  env.pManager->SetLoader<AnimationClip>( new AnimationClipLoader() ).Load<AnimationClip>();

  env.pManager->SetLoader<AnimationController>( new AnimationControllerLoader() ).Load<AnimationController>();

  env.eventManager = new EventManager;

  env.pECS = new ECSModule();


  //env.pECS->GetWorld().Load( "Base.json" );
}

Application::~Application()
{
  DeltaEngine_CORE_INFO( "Engine Shutdown" );
  //env.pECS->GetWorld().Save("Base.json");

  env.pECS->GetWorld().ShutdownSystems();
  delete env.pECS;
  delete env.eventManager;
  delete env.pManager;
  RenderModule::openGLSystem->Exit();
  delete RenderModule::openGLSystem;
  delete m_Editor;
  delete env.pWin;
  delete env.pClock;

  AudioEngine::Shutdown();
}


void Application::Run()
{

  /*DeltaEngine::World &world = env.pECS->GetWorld();
  DeltaEngine::EntityManager &em = world.GetEntityManager();
  auto entitybg = em.CreateEntity<Transform, Renderer2D, Image>();
  auto &spriterender = em.GetComponent<Image>( entitybg );
  auto entitysr = em.CreateEntity<Transform, Renderer2D, Image, Animator>();
  em.AddComponent<State>(entitysr);
  auto &animator = em.GetComponent<Animator>( entitysr );
  auto entitytr = em.CreateEntity<Transform, Renderer2D, Text>();
  auto &textrender = em.GetComponent<Text>( entitytr );
  auto &textrenderer = em.GetComponent<Renderer2D>( entitytr );
  spriterender.m_Sprite = { "Textures/bg", 0 };
  textrender.m_FontKey = "Default";
  textrenderer.m_Material = { "DefaultText" };
  animator.m_ControllerKey = "Animation/Player";
  textrender.m_Text = "Welcome to DELTA";
  textrender.alignment = Alignment::AlignRight;*/

  while ( env.pWin->Running() )
  {
    env.pClock->Update();
    InputManager::Get()->Update();
    // Logic Update()
    // Physics Update()
    // Animation Update()
    // Render Update()
    // Physics Update()
    env.pECS->GetWorld().Update();
    env.pECS->GetWorld().LateUpdate();
    m_Editor->Begin();
    m_Editor->Render();
    m_Editor->End();
    ::SwapBuffers( RenderModule::openGLSystem->GetWindowContext() );
    env.pWin->Update();

    OnEvent();
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