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
  env.pManager->SetLoader<Font>( new FontLoader() )
    .Load<Font>( "Fail", "Fonts/Arials.ttf" )
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

  //EntityID first = env.pECS->GetWorld().GetEntityManager().CreateEntity();
  //env.pECS->GetWorld().GetEntityManager().AddComponent<Collider>(first);
  //env.pECS->GetWorld().GetEntityManager().AddComponent<RigidBody>(first);
  //env.pECS->GetWorld().GetEntityManager().AddComponent<Transform>(first);
  //env.pECS->GetWorld().GetEntityManager().AddComponent<EntityType>(first);
  //env.pECS->GetWorld().GetEntityManager().AddComponent<Input>(first);
  //env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(first).type = "player";
  //env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(first).size = { 0.5,0.5 };
  //env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(first).type = ColliderType::BOX;
  //env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(first).position = { 0.5,0.5,0 };
  //env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(first).scale = { 0.5,0.5,0 };
  //env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(first).Movespeed = 100.0f;
  //env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(first).Mass = 15.0f;
  //env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(first).isMoveable = true;
  //env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(first).FrictionCoeff = 0.9f;
  //
  //EntityID sec = env.pECS->GetWorld().GetEntityManager().CreateEntity();
  //env.pECS->GetWorld().GetEntityManager().AddComponent<AI>(sec);
  //env.pECS->GetWorld().GetEntityManager().AddComponent<Collider>(sec);
  //env.pECS->GetWorld().GetEntityManager().AddComponent<RigidBody>(sec);
  //env.pECS->GetWorld().GetEntityManager().AddComponent<Transform>(sec);
  //env.pECS->GetWorld().GetEntityManager().AddComponent<EntityType>(sec);
  //env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(sec).key = "idle_monster";
  //env.pECS->GetWorld().GetEntityManager().GetComponent<AI>(sec).transition = "null";
  //env.pECS->GetWorld().GetEntityManager().GetComponent<EntityType>(sec).type = "monster";
  //env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(sec).size = { 0.5,0.5 };
  //env.pECS->GetWorld().GetEntityManager().GetComponent<Collider>(sec).type = ColliderType::BOX;
  //env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(sec).position = { 5,0,0 };
  //env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(sec).scale = { 0.5,0.5,0 };
  //env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(sec).Movespeed = 50.0f;
  //env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(sec).Mass = 20.0f;
  //env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(sec).isMoveable = true;
  //env.pECS->GetWorld().GetEntityManager().GetComponent<RigidBody>(sec).FrictionCoeff = 0.9f;

  env.pECS->GetWorld().CreateSystems<InputSystem, AISystem, PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem, PhysicsDrawSystem>();
  env.pECS->GetWorld().SetUpdateSequence<InputSystem, AISystem, PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem, PhysicsDrawSystem>();
  env.pECS->GetWorld().SetLateUpdateSequence<PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem, PhysicsDrawSystem>();




  env.pECS->GetWorld().InitSystems();
}

Application::~Application()
{
  DeltaEngine_CORE_INFO( "Engine Shutdown" );
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
  env.pManager->Get<Texture2D>( "run" )->SliceAll( 2, 3 );

      //auto* s = new SpriteRenderer(env.pManager->get<Texture2D>("run"),
      //    env.pManager->get<Shader>("Default"));
  auto entitybg = em.CreateEntity<Transform, Renderer2D, Image>();
  auto &spriterender = em.GetComponent<Image>( entitybg );
  auto entitysr = em.CreateEntity<Transform, Renderer2D, Image, Animator, State>();
  auto &animator = em.GetComponent<Animator>( entitysr );
  auto entitytr = em.CreateEntity<Transform, Renderer2D, Text>();
  auto &textrender = em.GetComponent<Text>( entitytr );
  auto &textrenderer = em.GetComponent<Renderer2D>( entitytr );

  spriterender.m_Sprite = { "bg" };
  textrender.m_FontKey = "Default";
  textrenderer.m_Material = { "DefaultText" } ;
  animator.m_ControllerKey = "Player";

  //size_t i = AudioEngine::PlaySound( "Audio/jump.wav" );

  while ( env.pWin->Running() )
  {
    textrender.m_Text = "FPS: " + std::to_string( static_cast<u32>( env.pClock->FrameRate() ) );
    if ( env.pClock->Update() )
    {
      InputManager::Get()->Update();
      // Update engine GameClock
      env.pECS->GetWorld().Update();
      env.pECS->GetWorld().LateUpdate();
      m_Editor->Begin();
      m_Editor->Render();
      m_Editor->End();
      ::SwapBuffers( RenderModule::openGLSystem->GetWindowContext() );
      env.pWin->Update();
      //if (!AudioEngine::IsChannelPlaying(i))
      //  i = AudioEngine::PlaySound( "Audio/jump.wav" );
      AudioEngine::Update();
      OnEvent();
    }
  }
}



void Application::OnEvent()
{
    if (!env.eventManager->IsEmpty())
    {
        auto ref = env.eventManager->ResolveEvent();
        EventDispatcher d(ref);
    
        if (ref != nullptr)
        {
            EventType type = ref->GetEventType();
            switch (type)
            {
            case EventType::ImGuiDragFile:
            {
                d.Dispatch<ImGuiFileDragEvent>(DE_BIND_EVENT_FN(Editor::OnDragDrop));
                break;
            }
            case EventType::ImGuiRemovingDragFile:
            {
                d.Dispatch<ImGuiFileRemovingDragEvent>(DE_BIND_EVENT_FN(Editor::OnRemovingDragDrop));
                break;
            }
            case EventType::ImGuiFileDragDone:
            {
                d.Dispatch<ImGuiFileDragEventDone>(DE_BIND_EVENT_FN(Editor::OnDragDropDone));
                break;
            }
            }
            delete ref;
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