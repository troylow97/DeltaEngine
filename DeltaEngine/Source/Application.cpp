#include "DeltaEngine.h"
#include "Application.h"

#include "EngineConfig.h"
#include "Render/OpenGLSystem.h"
#include "Render/TextRenderer.h"
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
  env.pECS->GetWorld().create_systems<InputSystem,AISystem, PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem, PhysicsDrawSystem>();
  env.pECS->GetWorld().set_update_sequence<InputSystem, AISystem, PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem, PhysicsDrawSystem>();
  env.pECS->GetWorld().set_late_update_sequence<PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem, PhysicsDrawSystem>();
  //env.pECS->GetWorld().Load("World/Entities.json");
  //EntityID first = env.pECS->GetWorld().get_entity_manager().CreateEntity();
  //env.pECS->GetWorld().get_entity_manager().AddComponent<Transform>(first);
  //env.pECS->GetWorld().get_entity_manager().AddComponent<Collider>(first);
  //env.pECS->GetWorld().get_entity_manager().AddComponent<RigidBody>(first);  
  //env.pECS->GetWorld().get_entity_manager().AddComponent<Input>(first);
  //
  //EntityID second = env.pECS->GetWorld().get_entity_manager().CreateEntity();
  //env.pECS->GetWorld().get_entity_manager().AddComponent<Transform>(second);
  //env.pECS->GetWorld().get_entity_manager().AddComponent<Collider>(second);
  //env.pECS->GetWorld().get_entity_manager().AddComponent<RigidBody>(second);
  //env.pECS->GetWorld().get_entity_manager().AddComponent<AI>(second);
  //
  //env.pECS->GetWorld().get_entity_manager().GetComponent<Transform>(first).position = { 0,0,0 };
  //env.pECS->GetWorld().get_entity_manager().GetComponent<Transform>(first).scale = { 0.5,0.5,0 };
  //env.pECS->GetWorld().get_entity_manager().GetComponent<RigidBody>(first).isMoveable = false;
  //env.pECS->GetWorld().get_entity_manager().GetComponent<RigidBody>(first).Mass = 10.0f;
  //env.pECS->GetWorld().get_entity_manager().GetComponent<Collider>(first).type = ColliderType::BOX;
  //
  //env.pECS->GetWorld().get_entity_manager().GetComponent<Transform>(second).position = { 2,3,0 };
  //env.pECS->GetWorld().get_entity_manager().GetComponent<RigidBody>(second).isMoveable = false;
  //env.pECS->GetWorld().get_entity_manager().GetComponent<Collider>(second).type = ColliderType::BOX;    
  //env.pECS->GetWorld().get_entity_manager().GetComponent<AI>(second).key = "idle";
  //env.pECS->GetWorld().Save("World/Entities.json");

}

Application::~Application()
{
  DeltaEngine_CORE_INFO( "Engine Shutdown" );

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
  DeltaEngine::EntityManager &em = world.get_entity_manager();
  env.pManager->Get<Texture2D>( "run" )->SliceAll( 2, 3 );

      //auto* s = new SpriteRenderer(env.pManager->get<Texture2D>("run"),
      //    env.pManager->get<Shader>("Default"));
  auto entitybg = em.CreateEntity<Transform, SpriteRenderer>();
  auto &spriterender = em.GetComponent<SpriteRenderer>( entitybg );
  auto entitysr = em.CreateEntity<Transform, SpriteRenderer, Animator>();
  auto &animator = em.GetComponent<Animator>( entitysr );
  auto entitytr = em.CreateEntity<Transform, TextRenderer>();
  auto &textrender = em.GetComponent<TextRenderer>( entitytr );
  auto entityps = em.CreateEntity<Transform, ParticleSystem>();

  spriterender.sprite = { "bg" };
  textrender.font = env.pManager->Get<Font>( "Default" );
  textrender.shader = env.pManager->Get<Shader>( "DefaultText" );
  textrender.transform.scale = Vector3( 0.75, 0.75 );
  animator.m_Controller = env.pManager->Get<AnimationController>( "Player" );

  //size_t i = AudioEngine::PlaySound( "Audio/jump.wav" );

  while ( env.pWin->Running() )
  {
    /*textrender.text = "FPS: " + std::to_string( static_cast<u32>( env.pClock->FrameRate() ) );
    textrender.transform.position = Vector3( ( Camera::editorCamera->Max().x - Camera::editorCamera->Min().x ) * -0.28f, ( Camera::editorCamera->Max().y - Camera::editorCamera->Min().y ) * 0.27f );
    */
    if ( env.pClock->Update() )
    {
      InputManager::Get()->Update();
      // Update engine GameClock
      env.pECS->GetWorld().update();
      env.pECS->GetWorld().late_update();
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
        }
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