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
#include "ECS/Components/Character.h"
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

  JsonFile f;
  EngineConfig c;
  f.StartReader( "config.json" ).LoadObject( c ).EndReader();
  env.pClock = new GameClock( c.fps );

  env.pWin = new Window( c.win_name, c.width, c.height );
  env.pWin->Init();

  // Render + Imgui
  RenderModule::openGLSystem = new RenderModule::OpenGLSystem();
  RenderModule::openGLSystem->Init();
  m_ImGuiLayer = new ImGuiLayer();
  m_ImGuiLayer->OnAttach();

  // Asset Loading
  env.pManager = new AM();
  env.pManager->set_loader<Font>( new FontLoader() )
    .load<Font>( "Fail", "Fonts/Arials.ttf" )
    .set_fallback<Font>( new Font("Fonts/Arial.ttf") );

  env.pManager->set_loader<Shader>( new ShaderLoader() )
    .load<Shader>( "Default", "Shaders/Default" )
    .load<Shader>( "DefaultText", "Shaders/DefaultText" )
    .set_fallback<Shader>( new Shader("Shaders/ErrorShader") );

  env.pManager->set_loader<Texture2D>( new TextureLoader() )
    .load<Texture2D>( "idle", "idle.png" )
    .load<Texture2D>( "run", "run.png" );

  env.pManager->set_loader<AnimationClip>( new AnimationClipLoader() )
    .load<AnimationClip>( "Idle", "Idle.clip" )
    .load<AnimationClip>( "Running", "Running.clip" );

  env.pManager->set_loader<AnimationController>( new AnimationControllerLoader() )
    .load<AnimationController>( "Player", "Player.anim" );

  env.pECS = new ECSModule();
  env.pECS->world();
  env.pECS->world().create_systems<InputSystem, PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem, PhysicsDrawSystem>();
  env.pECS->world().set_update_sequence<InputSystem, PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem, PhysicsDrawSystem>();
  env.pECS->world().set_late_update_sequence<PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem, PhysicsDrawSystem>();
  env.pECS->world().Load( "World/Entities.json" );
}

Application::~Application()
{
  DeltaEngine_CORE_INFO( "Engine Shutdown" );

  delete env.pECS;
  delete env.pManager;
  m_ImGuiLayer->OnDetach();
  RenderModule::openGLSystem->Exit();
  delete RenderModule::openGLSystem;
  delete env.pWin;
  delete env.pClock;
}


void Application::Run()
{
  DeltaEngine::World &world = env.pECS->world();
  DeltaEngine::EntityManager &em = world.get_entity_manager();
  env.pManager->get<Texture2D>( "run" )->SliceAll( 2, 3 );

  //auto* s = new SpriteRenderer(env.pManager->get<Texture2D>("run"),
  //    env.pManager->get<Shader>("Default"));
  auto entitysr = env.pECS->world().get_entity_manager().create_entity<Transform, SpriteRenderer, Animator>();
  auto &animator = env.pECS->world().get_entity_manager().get_component<Animator>( entitysr );
  auto entitytr = env.pECS->world().get_entity_manager().create_entity<Transform, TextRenderer>();
  auto &textrender = env.pECS->world().get_entity_manager().get_component<TextRenderer>( entitytr );
  auto entityps = env.pECS->world().get_entity_manager().create_entity<Transform, ParticleSystem>();

  textrender.font = env.pManager->get<Font>( "Default" );
  textrender.shader = env.pManager->get<Shader>( "DefaultText" );
  textrender.transform.scale = Vector3( 0.75, 0.75 );
  animator.m_Controller = env.pManager->get<AnimationController>( "Player" );

  while ( env.pWin->Running() )
  {
    textrender.text = "FPS: " + std::to_string( static_cast<u32>( env.pClock->FrameRate() ) );
    textrender.transform.position = Vector3( ( Camera::editorCamera->Max().x - Camera::editorCamera->Min().x ) * -0.28, ( Camera::editorCamera->Max().y - Camera::editorCamera->Min().y ) * 0.27f );
    if ( env.pClock->Update() )
    {
      InputManager::get()->update();
      env.pWin->Update();
      // Update engine GameClock
      env.pECS->world().update();
      env.pECS->world().late_update();
      m_ImGuiLayer->Begin();
      m_ImGuiLayer->End();
      ::SwapBuffers( RenderModule::openGLSystem->GetWindowContext() );
    }
  }
}


void Application::OnEvent()
{
    //EventManager event_manager;

    //event_manager.addEvent(WindowCloseEvent());

    //if (!event_manager.isEmpty())
    //{
    //    auto& ref = event_manager.resolveEvent();
    //    EventDispatcher d(ref);
    //    d.Dispatch<WindowCloseEvent>(DE_BIND_EVENT_FN(Application::OnWindowClose));
    //}
}


bool Application::OnWindowResize( WindowResizeEvent &e )
{
  if ( e.GetWidth() == 0 || e.GetHeight() == 0 )
  {
    m_Minimized = true;
    return false;
  }

  m_Minimized = false;
  //Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

  return false;
}
}