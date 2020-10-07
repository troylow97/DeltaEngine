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
#include "Input/InputSystem.h"
#include "Input/Keys.h"
/*-----------------------------------
#include "Event/ApplicationEvent.h"
#include "Log.h"
-----------------------------------*/
namespace DeltaEngine
{
    DeltaEngineGlobalEnvironment env;

    Application::Application() : m_Running{ true }, m_Minimized{ true }, m_interval(0.25)
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

        // Render + Imgui
        RenderModule::openGLSystem = new RenderModule::OpenGLSystem();
        RenderModule::openGLSystem->Init();
        m_ImGuiLayer = new ImGuiLayer();
        m_ImGuiLayer->OnAttach();

        // Asset Loading
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

        env.pManager->set_loader<AnimationController>(new AnimationControllerLoader())
            .load<AnimationController>("Player", "Player.anim");

        env.pECS = new ECSModule();
        env.pECS->world();
        env.pECS = new ECSModule();
        env.pECS->world();
        env.pECS->world().create_systems<PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem, PhysicsDrawSystem>();
        env.pECS->world().set_update_sequence<PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem, PhysicsDrawSystem>();
        env.pECS->world().set_late_update_sequence<PhysicsSystem, CollisionSystem, AnimationSystem, RenderSystem, PhysicsDrawSystem>();
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
        DeltaEngine::World& world = env.pECS->world();
        DeltaEngine::EntityManager& em = world.get_entity_manager();
        env.pManager->get<Texture2D>("run")->SliceAll(2, 3);

        //auto* s = new SpriteRenderer(env.pManager->get<Texture2D>("run"),
        //    env.pManager->get<Shader>("Default"));
        auto entityss = env.pECS->world().get_entity_manager().create_entity<Transform, SpriteRenderer, Animator>();
        //auto& spriteRenderer = env.pECS->world().get_entity_manager().get_component<SpriteRenderer>(entity1);
        auto& animator = env.pECS->world().get_entity_manager().get_component<Animator>(entityss);

        //spriteRenderer.shader = env.pManager->get<Shader>("Default");
        animator.m_Controller = env.pManager->get<AnimationController>("Player");
        //animator.renderer = &spriteRenderer;

        //auto* a = new Animator(env.pManager->get<AnimationController>("Player"));
        //a->renderer = s;

        //auto* t = new TextRenderer(env.pManager->get<Font>("Fail"),
        //    env.pManager->get<Shader>("DefaultText"));
        //auto* p = new ParticleSystem();

        //physics test start init var
        auto entity1 = env.pECS->world().get_entity_manager().create_entity<Transform, Collider>();
        auto entity2 = env.pECS->world().get_entity_manager().create_entity<Transform, RigidBody, Collider>();
        auto& trans = env.pECS->world().get_entity_manager().get_component<Transform>(entity1);
        auto& col = env.pECS->world().get_entity_manager().get_component<Collider>(entity1);

        trans.position = Vector3(0.55f, 0.55f);
        trans.scale = Vector3(1, 1);
        col.type = ColliderType::BOX;

        auto& t2 = env.pECS->world().get_entity_manager().get_component<Transform>(entity2);
        auto& r2 = env.pECS->world().get_entity_manager().get_component<RigidBody>(entity2);
        auto& col2 = env.pECS->world().get_entity_manager().get_component<Collider>(entity1);

        t2.position = Vector3(-0.55f, -0.55f);
        t2.scale = Vector3(1, 1);
        r2.Velocity = Vector2(0, 0);
        col2.type = ColliderType::BOX;
        //physics test end
        // TODO Modules Instantiation
        f64 accumulator = 0.0;

        MSG msg = {};
        while (m_Running)
        {
            if (InputSystem::get()->isKeyPressed(DEVK_A))
            {
                env.pECS->world().get_entity_manager().for_each([&](EntityID id1, RigidBody& r1)
                    {
                        r1.Velocity = {-1, 0};
                    });
            }
            if (InputSystem::get()->isKeyPressed(DEVK_D))
            {
                env.pECS->world().get_entity_manager().for_each([&](EntityID id1, RigidBody& r1)
                    {
                        r1.Velocity = {1, 0};
                    });
            }
            if (InputSystem::get()->isKeyPressed(DEVK_W))
            {
                env.pECS->world().get_entity_manager().for_each([&](EntityID id1, RigidBody& r1)
                    {
                        r1.Velocity = {0, 1};
                    });
            }
            if (InputSystem::get()->isKeyPressed(DEVK_S))
            {
                env.pECS->world().get_entity_manager().for_each([&](EntityID id1, RigidBody& r1)
                    {
                        r1.Velocity = {0, -1};
                    });
            }
            InputSystem::get()->update();
            // Update engine GameClock
            env.pClock->Update();
            env.pECS->world().update();
            //m_ImGuiLayer->Begin();
            //m_ImGuiLayer->End();
            env.pECS->world().late_update();
            // Update accumulator using time-scaled dt
            accumulator += env.pClock->DeltaTime();

            // Update based on interval
            while (accumulator >= m_interval)
            {
                if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
                {
                    if (msg.message == WM_QUIT)
                        m_Running = false;
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                    continue;
                }
                env.pWin->Update();
                VariableUpdate();
                FixedUpdate();
                accumulator -= env.pClock->DeltaTime();
            }
            const f64 alpha = accumulator / m_interval;
        }
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
        UNREFERENCED_PARAMETER(e);
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
        // GUI Update
        // Memory Update
    }
}
