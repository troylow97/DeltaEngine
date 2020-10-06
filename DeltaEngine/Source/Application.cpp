#include "DeltaEngine.h"
#include "Application.h"
#include "Render/OpenGLSystem.h"
#include "Render/TextRenderer.h"
#include "Core/Utils/FileUtils.h"
#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"
#include "Physics/PhysicsSystem.h"
#include "Physics/CollisionSystem.h"
#include "ECS/World.h"
#include "Input/InputSystem.h"
#include "Input/Keys.h"
/*-----------------------------------
#include "Event/ApplicationEvent.h"
#include "Log.h"
-----------------------------------*/
namespace DeltaEngine
{
    DeltaEngineGlobalEnvironment env;

    Application::Application() : m_interval(0.25)
    {
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

        // a lot of this should be moved to a function in GraphicsManager later
        RenderModule::openGLSystem = new RenderModule::OpenGLSystem();
        RenderModule::openGLSystem->Init();

        FileUtils::Root("Assets");
        env.pClock = new GameClock();
        env.pManager = new AM();
        env.pManager->set_loader<Font>(new FontLoader())
            .load<Font>("Default", "Fonts/Arial.ttf")
            .load<Font>("Fail", "Fonts/Arials.ttf")
            .set_fallback<Font>(env.pManager->get<Font>("Default"));

        env.pManager->set_loader<Shader>(new ShaderLoader())
            .load<Shader>("Default", "Shaders/Default")
            .load<Shader>("DefaultText", "Shaders/DefaultText");

        env.pManager->set_loader<Texture2D>(new TextureLoader())
            .load<Texture2D>("idle", "idle.png")
            .load<Texture2D>("run", "run.png");

        env.pManager->set_loader<AnimationClip>(new AnimationClipLoader())
            .load<AnimationClip>("Idle", "Idle.clip")
            .load<AnimationClip>("Running", "Running.clip");

        env.pManager->set_loader<Animator>(new AnimatorLoader())
            .load<Animator>("Player", "Player.anim");

        env.pECS = new ECSModule();
        env.pECS->world();
    }
    env.pECS = new ECSModule();
    env.pECS->world();
    env.pECS->world().create_systems<PhysicsSystem, CollisionSystem>();
    env.pECS->world().set_update_sequence<PhysicsSystem, CollisionSystem>();

  }

    Application::~Application()
    {
        DeltaEngine_CORE_INFO("Engine Shutdown");
        // Events
        // Audio
        // Physics
        // GUI
        // Render
        RenderModule::openGLSystem->Exit();
        delete RenderModule::openGLSystem;
        // Window
        // Memory Manager
    }

    void Application::Run()
    {
        DeltaEngine::World& world = env.pECS->world();
        DeltaEngine::EntityManager& em = world.get_entity_manager();
        env.pManager->get<Texture2D>("run")->SliceAll(2, 3);

        auto* s = new SpriteRenderer(env.pManager->get<Texture2D>("run"),
            env.pManager->get<Shader>("Default"));
        //auto* t = new TextRenderer(env.pManager->get<Font>("Fail"),
        //    env.pManager->get<Shader>("DefaultText"));
        //auto* p = new ParticleSystem();

    anim = new FrameAnimation();
    anim->renderer = s;
    //physics test start init var
    auto entity1 = env.pECS->world().get_entity_manager().create_entity<Transform, Collider>();
    auto entity2 = env.pECS->world().get_entity_manager().create_entity<Transform,RigidBody, Collider>();
    auto& trans = env.pECS->world().get_entity_manager().get_component<Transform>(entity1);
    auto& col = env.pECS->world().get_entity_manager().get_component<Collider>(entity1);
        //anim = new FrameAnimation();
        //anim->renderer = s;

    trans.position = { 7,7,0 };
    trans.scale = { 1,1,0 };
    col.type = ColliderType::BOX;

    auto& t2 = env.pECS->world().get_entity_manager().get_component<Transform>(entity2);
    auto& r2 = env.pECS->world().get_entity_manager().get_component<RigidBody>(entity2);
    auto& col2 = env.pECS->world().get_entity_manager().get_component<Collider>(entity1);

    t2.position = { 5,5,0 };
    t2.scale = { 1,1,0 };
    r2.Velocity = { 0,0 };
    col2.type = ColliderType::BOX;
    //physics test end
    // TODO Modules Instantiation
    f64 accumulator = 0.0;

    MSG msg = {};
    bool isRunning{true};
    while (isRunning)
    {
      if (InputSystem::get()->isKeyTriggered(DEVK_D))
      {
          env.pECS->world().get_entity_manager().for_each([&](EntityID id1, RigidBody& r1)
          {
                  r1.Velocity += {1, 0};
          });
      }
      if (InputSystem::get()->isKeyPressed(DEVK_D))
      {
          env.pECS->world().get_entity_manager().for_each([&](EntityID id1, RigidBody& r1)
              {
                  r1.Velocity += {0.2, 0};
              });
      }
      InputSystem::get()->update();
      // Update engine GameClock
      env.pClock->Update();
      env.pECS->world().update();
      // Update accumulator using time-scaled dt
      accumulator += env.pClock->DeltaTime();

            // Update based on interval
            while (accumulator >= m_interval)
            {
                env.pWin->Update();
                VariableUpdate();
                FixedUpdate();
                accumulator -= env.pClock->DeltaTime();
            }
            const f64 alpha = accumulator / m_interval;
            std::cout << "aA";
        }
        delete s;
        //delete t;
        //delete p;
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::OnEvent()
    {
        EventManager event_manager;

        event_manager.addEvent(WindowCloseEvent());

        if (!event_manager.isEmpty())
        {
            auto& ref = event_manager.resolveEvent();
            EventDispatcher d(ref);
            d.Dispatch<WindowCloseEvent>(DE_BIND_EVENT_FN(Application::OnWindowClose));
        }

        //below is how cherno does it
        //EventDispatcher dispatcher(e);
        //dispatcher.Dispatch<WindowCloseEvent>(DE_BIND_EVENT_FN(Application::OnWindowClose));
        //dispatcher.Dispatch<WindowResizeEvent>(DE_BIND_EVENT_FN(Application::OnWindowResize));
        //
        //for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
        //{
        //    if (e.isHandled)
        //        break;
        //    (*it)->OnEvent(e);
        //}
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
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
        //anim->Update(env.pClock->DeltaTime());
        RenderModule::openGLSystem->Update();
        // GUI Update
        // Memory Update
    }
}
