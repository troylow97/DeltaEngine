#include "DeltaEngine.h"
#include "Application.h"
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
#include "ECS/World.h"
#include "ECS/Components/Character.h"
#include "Input/InputSystem.h"
#include "Input/Keys.h"
#include "Reflect/Reflect.h"

/*-----------------------------------
#include "Event/ApplicationEvent.h"
#include "Log.h"
-----------------------------------*/
namespace DeltaEngine
{
    DeltaEngineGlobalEnvironment env;

    Application::Application() : m_Running{ true }, m_Minimized{ true }, m_interval(0.25)
    {
        FileUtils::Root("Assets");
        Log::Init();
        DeltaEngine_CORE_INFO("Engine Start");
        // Memory Manager
        // Window
        // Render
        // GUI
        // Physics
        // Audio
        // Events
        env.pWin = new Window();
        env.pWin->Init();

        // Render + Imgui
        RenderModule::openGLSystem = new RenderModule::OpenGLSystem();
        RenderModule::openGLSystem->Init();
        m_ImGuiLayer = new ImGuiLayer();
        m_ImGuiLayer->OnAttach();

        // Asset Loading
        env.pClock = new GameClock();
        env.pManager = new AM();
        env.pManager->set_loader<Font>(new FontLoader())
            .load<Font>("Default", "Fonts/Arial.ttf")
            .load<Font>("Fail", "Fonts/Arials.ttf")
            .set_fallback<Font>(env.pManager->get<Font>("Default"));

        env.pManager->set_loader<Shader>(new ShaderLoader())
            .load<Shader>("Default", "Shaders/Default")
            .load<Shader>("DefaultText", "Shaders/DefaultText")
            .load<Shader>("Error", "Shaders/ErrorShader")
            .set_fallback<Shader>(env.pManager->get<Shader>("Error"));

        env.pManager->set_loader<Texture2D>(new TextureLoader())
            .load<Texture2D>("idle", "idle.png")
            .load<Texture2D>("run", "run.png")
            .load<Texture2D>("bg", "bg.png");

        env.pManager->set_loader<AnimationClip>(new AnimationClipLoader())
            .load<AnimationClip>("Idle", "Idle.clip")
            .load<AnimationClip>("Running", "Running.clip");
    env.pECS = new ECSModule();
    env.pECS->world().update();

        env.pManager->set_loader<AnimationController>(new AnimationControllerLoader())
            .load<AnimationController>("Player", "Player.anim");

        env.pECS = new ECSModule();
        env.pECS->world();
        env.pECS->world().create_systems<PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem, PhysicsDrawSystem>();
        env.pECS->world().set_update_sequence<PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem, PhysicsDrawSystem>();
        env.pECS->world().set_late_update_sequence<PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem, PhysicsDrawSystem>();
        env.pECS->world().Load("Entities.json"); 
    }

    Application::~Application()
    {
        DeltaEngine_CORE_INFO("Engine Shutdown");
        // Events
        // Audio
        // Physics
        // GUI
        m_ImGuiLayer->OnDetach();
        // Render
        RenderModule::openGLSystem->Exit();
        delete RenderModule::openGLSystem;
        // Window
        // Memory Manager
    }


    void Application::Run()
    {
      DeltaEngine::World &world = env.pECS->world();
      DeltaEngine::EntityManager &em = world.get_entity_manager();
      env.pManager->get<Texture2D>( "run" )->SliceAll( 2, 3 );

      //auto* s = new SpriteRenderer(env.pManager->get<Texture2D>("run"),
      //    env.pManager->get<Shader>("Default"));
      auto entitybg = env.pECS->world().get_entity_manager().create_entity<Transform, SpriteRenderer>();
      auto& spriterender = env.pECS->world().get_entity_manager().get_component<SpriteRenderer>(entitybg);
      auto entitysr = env.pECS->world().get_entity_manager().create_entity<Transform, SpriteRenderer, Animator>();
      auto &animator = env.pECS->world().get_entity_manager().get_component<Animator>( entitysr );
      auto entitytr = env.pECS->world().get_entity_manager().create_entity<Transform, TextRenderer>();
      auto &textrender = env.pECS->world().get_entity_manager().get_component<TextRenderer>( entitytr );
      auto entityps = env.pECS->world().get_entity_manager().create_entity<Transform, ParticleSystem>();

      spriterender.sprite = {"bg"};
      textrender.font = env.pManager->get<Font>( "Default" );
      textrender.shader = env.pManager->get<Shader>( "DefaultText" );
      textrender.transform.scale = Vector3( 0.75, 0.75 );
      animator.m_Controller = env.pManager->get<AnimationController>( "Player" );

      // FPS Limiter
      using frames = std::chrono::duration<i32, std::ratio<1, 60>>;
      auto nextFrame = std::chrono::system_clock::now() + frames{ 0 };
      auto lastFrame = nextFrame - frames{ 1 };

        while ( env.pWin->Running() )
        {
          // FPS Limiter
          std::this_thread::sleep_until( nextFrame );
          lastFrame = nextFrame;
          nextFrame += frames { 1 };

          env.pClock->Update();

          textrender.text = "FPS: " + std::to_string( static_cast<u32>( env.pClock->FrameRate() ) );
          textrender.transform.position = Vector3( ( Camera::editorCamera->Max().x - Camera::editorCamera->Min().x ) * -0.28, ( Camera::editorCamera->Max().y - Camera::editorCamera->Min().y ) * 0.27f );

          InputSystem::get()->update();
          if ( InputSystem::get()->isKeyPressed( DEVK_A ) )
          {
            env.pECS->world().get_entity_manager().for_each( [&]( EntityID id1, RigidBody &r1, Input &i1 )
            {
              i1.previousKey = DEVK_A;
              r1.Velocity = { -1, 0 };
            } );
            env.pECS->world().get_entity_manager().for_each( [&]( EntityID id1, Animator &a )
            {
              a.SetFloat( "Speed", 1.0f );
            } );
            env.pECS->world().get_entity_manager().for_each( [&]( EntityID id1, SpriteRenderer &s )
            {
              s.m_FlipX = true;
            } );
          }
          else if ( InputSystem::get()->isKeyReleased( DEVK_A ) )
          {
            env.pECS->world().get_entity_manager().for_each( [&]( EntityID id1, Animator &a )
            {
              a.SetFloat( "Speed", 0.0f );
            } );
          }
          if ( InputSystem::get()->isKeyPressed( DEVK_D ) )
          {
            env.pECS->world().get_entity_manager().for_each( [&]( EntityID id1, RigidBody &r1, Input &i1 )
            {
              i1.previousKey = DEVK_D;
              r1.Velocity = { 1, 0 };
            } );
            env.pECS->world().get_entity_manager().for_each( [&]( EntityID id1, Animator &a )
            {
              a.SetFloat( "Speed", 1.0f );
            } );
            env.pECS->world().get_entity_manager().for_each( [&]( EntityID id1, SpriteRenderer &s )
            {
              s.m_FlipX = false;
            } );
          }
          else if ( InputSystem::get()->isKeyReleased( DEVK_D ) )
          {
            env.pECS->world().get_entity_manager().for_each( [&]( EntityID id1, Animator &a )
            {
              a.SetFloat( "Speed", 0.0f );
            } );
          }
          if ( InputSystem::get()->isKeyPressed( DEVK_W ) )
          {
            env.pECS->world().get_entity_manager().for_each( [&]( EntityID id1, RigidBody &r1, Input &i1 )
            {
              i1.previousKey = DEVK_W;
              r1.Velocity = { 0, 1 };
            } );
          }
          if ( InputSystem::get()->isKeyPressed( DEVK_S ) )
          {
            env.pECS->world().get_entity_manager().for_each( [&]( EntityID id1, RigidBody &r1, Input &i1 )
            {
              i1.previousKey = DEVK_S;
              r1.Velocity = { 0, -1 };
            } );
          }
          if ( InputSystem::get()->isKeyTriggered( DEVK_BACKSLASH ) ) // '\'
          {
            if ( InputSystem::get()->getShowLine() == false )
            {
                //std::cout << "V is triggered and line is shown" << std::endl;
              InputSystem::get()->setShowLine( true );
            }
            else if ( InputSystem::get()->getShowLine() == true )
            {
                //std::cout << "V is triggered and line is NOT shown" << std::endl;
              InputSystem::get()->setShowLine( false );
            }
          }

          if ( InputSystem::get()->isKeyTriggered( DEVK_P ) )
          {
            auto circleEntity = env.pECS->world().get_entity_manager().create_entity<Transform, RigidBody, Collider>();
            auto &circleT = env.pECS->world().get_entity_manager().get_component<Transform>( circleEntity );
            auto &circleR = env.pECS->world().get_entity_manager().get_component<RigidBody>( circleEntity );
            auto &circleC = env.pECS->world().get_entity_manager().get_component<Collider>( circleEntity );

            circleT.scale = Vector3( 0.1, 0.1 );
            circleR.hasGravity = true;
            circleC.type = ColliderType::CIRCLE;
          }

          // gets the coordinates of the mouse, good for debugging
          if ( InputSystem::get()->onMouseMove() )
            InputSystem::get()->currentPosition();

          InputSystem::get()->update();
          env.pWin->Update();
          // Update engine GameClock
          env.pECS->world().update();
          env.pECS->world().late_update();
          m_ImGuiLayer->Begin();
          m_ImGuiLayer->End();
          ::SwapBuffers( RenderModule::openGLSystem->GetWindowContext() );
          // Update accumulator using time-scaled dt
          // accumulator += env.pClock->DeltaTime();
          //// Update based on interval
          //while (accumulator >= m_interval)
          //{
          //    accumulator -= env.pClock->DeltaTime();
          //}
          //const f64 alpha = accumulator / m_interval;
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


    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        if (e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            m_Minimized = true;
            return false;
        }

        m_Minimized = false;
        //Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

        return false;
    }
}
