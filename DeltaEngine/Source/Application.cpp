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

  // Asset Loading
  env.pManager = new AM();
  env.pManager->SetLoader<Font>( new FontLoader() )
    .Load<Font>( "Arial", "Fonts/Arial.ttf" )
    .SetFallback<Font>( new Font( "Fonts/Arial.ttf" ) );

  env.pManager->SetLoader<Shader>( new ShaderLoader() )
    .Load<Shader>( "Default", "Shaders/Default" )
    .Load<Shader>( "DefaultText", "Shaders/DefaultText" )
    .SetFallback<Shader>( new Shader( "Shaders/ErrorShader" ) );

  env.pManager->SetLoader<Texture2D>( new TextureLoader() )
    .Load<Texture2D>( "idle", "idle.png" )
    .Load<Texture2D>( "run", "run.png" )
    .Load<Texture2D>( "bg", "bg.png" );

  env.pManager->SetLoader<AnimationClip>( new AnimationClipLoader() )
    .Load<AnimationClip>( "Idle", "Idle.clip" )
    .Load<AnimationClip>( "Running", "Running.clip" );


  env.pManager->SetLoader<AnimationController>( new AnimationControllerLoader() )
    .Load<AnimationController>( "Player", "Player.anim" );

  env.eventManager = new EventManager;

  env.pECS = new ECSModule();
  env.pECS->GetWorld().CreateSystems<InputSystem, PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem, PhysicsDrawSystem>();
  env.pECS->GetWorld().SetUpdateSequence<InputSystem, PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem, PhysicsDrawSystem>();
  env.pECS->GetWorld().SetLateUpdateSequence<PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem, PhysicsDrawSystem>();
  env.pECS->GetWorld().InitSystems();
  env.pECS->GetWorld().Load( "Base.json" );
}

Application::~Application()
{
  DeltaEngine_CORE_INFO( "Engine Shutdown" );
  env.pECS->GetWorld().Save("Base.json");

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
  env.pManager->Get<Texture2D>( "run" )->SliceAll( 2, 3 );
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

  spriterender.m_Sprite = { "bg", 0 };
  textrender.m_FontKey = "Default";
  textrenderer.m_Material = { "DefaultText" };
  animator.m_ControllerKey = "Player";
  textrender.m_Text = "Welcome to DELTA";
  textrender.alignment = Alignment::AlignRight;*/
  //DeltaEngine_CORE_WARN( "ComponentMeta Size: {}, Object Size: {}", ComponentMeta::GetComponentMeta<Name>()->size, sizeof( Name ) );
  //DeltaEngine_CORE_WARN( "ComponentMeta Size: {}, Object Size: {}", ComponentMeta::GetComponentMeta<Parent>()->size, sizeof( Parent ) );
  //DeltaEngine_CORE_WARN( "ComponentMeta Size: {}, Object Size: {}", ComponentMeta::GetComponentMeta<Input>()->size, sizeof( Input ) );
  //DeltaEngine_CORE_WARN( "ComponentMeta Size: {}, Object Size: {}", ComponentMeta::GetComponentMeta<Transform>()->size, sizeof( Transform ) );
  //DeltaEngine_CORE_WARN( "ComponentMeta Size: {}, Object Size: {}", ComponentMeta::GetComponentMeta<RigidBody>()->size, sizeof( RigidBody ) );
  //DeltaEngine_CORE_WARN( "ComponentMeta Size: {}, Object Size: {}", ComponentMeta::GetComponentMeta<Collider>()->size, sizeof( Collider ) );
  //DeltaEngine_CORE_WARN( "ComponentMeta Size: {}, Object Size: {}", ComponentMeta::GetComponentMeta<Image>()->size, sizeof( Image ) );
  //DeltaEngine_CORE_WARN( "ComponentMeta Size: {}, Object Size: {}", ComponentMeta::GetComponentMeta<Renderer2D>()->size, sizeof( Renderer2D ) );
  //DeltaEngine_CORE_WARN( "ComponentMeta Size: {}, Object Size: {}", ComponentMeta::GetComponentMeta<Text>()->size, sizeof( Text ) );
  //DeltaEngine_CORE_WARN( "ComponentMeta Size: {}, Object Size: {}", ComponentMeta::GetComponentMeta<Animator>()->size, sizeof( Animator ) );
  //DeltaEngine_CORE_WARN( "ComponentMeta Size: {}, Object Size: {}", ComponentMeta::GetComponentMeta<State>()->size, sizeof( State ) );


  while ( env.pWin->Running() )
  {
    env.pClock->Update();
      InputManager::Get()->Update();
      // Update engine GameClock
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