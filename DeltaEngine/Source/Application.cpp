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

  EntityID first = env.pECS->GetWorld().GetEntityManager().CreateEntity();
  env.pECS->GetWorld().GetEntityManager().AddComponent<Collider>(first);
  env.pECS->GetWorld().GetEntityManager().AddComponent<RigidBody>(first);
  env.pECS->GetWorld().GetEntityManager().AddComponent<Transform>(first);
  env.pECS->GetWorld().GetEntityManager().AddComponent<EntityType>(first);
  env.pECS->GetWorld().GetEntityManager().AddComponent<Input>(first);
  env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(first).type = "player";
  env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(first).size = { 0.5,0.5 };
  env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(first).type = ColliderType::BOX;
  env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(first).position = { 0.5,2.0,0 };
  env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(first).scale = { 0.5,0.5,0 };
  env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(first).Movespeed = 100.0f;
  env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(first).Mass = 15.0f;
  env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(first).isMoveable = true;
  env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(first).FrictionCoeff = 0.9f;
  
  EntityID sec = env.pECS->GetWorld().GetEntityManager().CreateEntity();
  //env.pECS->GetWorld().GetEntityManager().AddComponent<AI>(sec);
  env.pECS->GetWorld().GetEntityManager().AddComponent<Collider>(sec);
  env.pECS->GetWorld().GetEntityManager().AddComponent<RigidBody>(sec);
  env.pECS->GetWorld().GetEntityManager().AddComponent<Transform>(sec);
  env.pECS->GetWorld().GetEntityManager().AddComponent<EntityType>(sec);
  env.pECS->GetWorld().GetEntityManager().AddComponent<AI>(sec);
  env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(sec).key = "idle_monster";
  env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(sec).transition = "null";
  env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(sec).type = "monster";
  env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(sec).size = { 0.5,0.5 };
  env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(sec).type = ColliderType::BOX;
  env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(sec).position = { -2,-3,0 };
  env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(sec).scale = { 0.5,0.5,0 };
  env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(sec).Movespeed = 50.0f;
  env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(sec).Mass = 20.0f;
  env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(sec).isMoveable = true;
  env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(sec).FrictionCoeff = 0.9f;

  EntityID third = env.pECS->GetWorld().GetEntityManager().CreateEntity();
  env.pECS->GetWorld().GetEntityManager().AddComponent<Collider>(third);
  env.pECS->GetWorld().GetEntityManager().AddComponent<RigidBody>(third);
  env.pECS->GetWorld().GetEntityManager().AddComponent<Transform>(third);
  env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(third).type = ColliderType::BOX;
  env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(third).position = { 0,-2,0 };
  env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(third).scale = { 10.5,0.5,0 };
  env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(third).Movespeed = 50.0f;
  env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(third).Mass = 1500.0f;
  env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(third).isMoveable = false;
  env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(third).FrictionCoeff = 0.9f;

  EntityID fourth = env.pECS->GetWorld().GetEntityManager().CreateEntity();
  env.pECS->GetWorld().GetEntityManager().AddComponent<Collider>(fourth);
  env.pECS->GetWorld().GetEntityManager().AddComponent<RigidBody>(fourth);
  env.pECS->GetWorld().GetEntityManager().AddComponent<Transform>(fourth);
  env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(fourth).type = ColliderType::BOX;
  env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(fourth).position = { 4,0,0 };
  env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(fourth).scale = { 0.5,10.5,0 };
  env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(fourth).Movespeed = 50.0f;
  env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(fourth).Mass = 1500.0f;
  env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(fourth).isMoveable = false;
  env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(fourth).FrictionCoeff = 0.9f;

  EntityID fifth = env.pECS->GetWorld().GetEntityManager().CreateEntity();
  env.pECS->GetWorld().GetEntityManager().AddComponent<Collider>(fifth);
  env.pECS->GetWorld().GetEntityManager().AddComponent<RigidBody>(fifth);
  env.pECS->GetWorld().GetEntityManager().AddComponent<Transform>(fifth);
  env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(fifth).type = ColliderType::BOX;
  env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(fifth).position = { -2,2,0 };
  env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(fifth).scale = { 0.5,0.5,0 };
  env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(fifth).Movespeed = 50.0f;
  env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(fifth).Mass = 50.0f;
  env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(fifth).isMoveable = true;
  env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(fifth).FrictionCoeff = 0.9f;

  EntityID sixth = env.pECS->GetWorld().GetEntityManager().CreateEntity();
  env.pECS->GetWorld().GetEntityManager().AddComponent<Collider>(sixth);
  env.pECS->GetWorld().GetEntityManager().AddComponent<RigidBody>(sixth);
  env.pECS->GetWorld().GetEntityManager().AddComponent<Transform>(sixth);
  env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(sixth).type = ColliderType::BOX;
  env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(sixth).position = { -2,3,0 };
  env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(sixth).scale = { 0.5,0.5,0 };
  env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(sixth).Movespeed = 50.0f;
  env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(sixth).Mass = 50.0f;
  env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(sixth).isMoveable = true;
  env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(sixth).FrictionCoeff = 0.9f;

  env.pECS->GetWorld().Save("Entities2.json");

  env.pECS->GetWorld().CreateSystems<InputSystem, AISystem, PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem, PhysicsDrawSystem>();
  env.pECS->GetWorld().SetUpdateSequence<InputSystem, AISystem, PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem, PhysicsDrawSystem>();
  env.pECS->GetWorld().SetLateUpdateSequence<PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem, PhysicsDrawSystem>();




  env.pECS->GetWorld().InitSystems();
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

  DeltaEngine::World &world = env.pECS->GetWorld();
  DeltaEngine::EntityManager &em = world.GetEntityManager();
  auto entitybg = em.CreateEntity<Transform, Renderer2D, Image>();
  auto &spriterender = em.GetComponent<Image>( entitybg );
  auto entitysr = em.CreateEntity<Transform, Renderer2D, Image, Animator, State>();
  auto &animator = em.GetComponent<Animator>( entitysr );
  auto entitytr = em.CreateEntity<Transform, Renderer2D, Text>();
  auto &textrender = em.GetComponent<Text>( entitytr );
  auto &textrenderer = em.GetComponent<Renderer2D>( entitytr );
  spriterender.m_Sprite = { "Textures/bg", 0 };
  textrender.m_FontKey = "Default";
  textrenderer.m_Material = { "DefaultText" };
  animator.m_ControllerKey = "Animation/Dave";
  textrender.m_Text = "Welcome to DELTA";
  textrender.alignment = Alignment::AlignRight;

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