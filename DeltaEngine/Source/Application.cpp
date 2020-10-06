#include "DEpch.h"
#include "DeltaEngine.h"
#include "Application.h"
#include "Render/OpenGLSystem.h"
#include "Render/TextRenderer.h"
#include "Event/ApplicationEvent.h"
#include "Physics/Collision.h"
#include "Core/Utils/FileUtils.h"
#include "Core/GlobalStruct.h"

/*-----------------------------------
#include "Event/ApplicationEvent.h"
#include "Log.h"
-----------------------------------*/
namespace DeltaEngine
{
    DeltaEngineGlobalEnvironment env;

    Application::Application() : m_Running{ true }, m_Minimized{ false }, m_interval(0.25)
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

        //anim = new FrameAnimation();
        //anim->renderer = s;

        // TODO Modules Instantiation
        f64 accumulator = 0.0;

        MSG msg = {};

        while (m_Running)
        {
            // Update engine GameClock
            env.pClock->Update();

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
                FixedUpdate();
                //a->Update();
                VariableUpdate();
                accumulator -= env.pClock->DeltaTime();
            }
            const f64 alpha = accumulator / m_interval;
        }
        delete s;
        //delete t;
        //delete p;
    }

    void Application::OnEvent(Event& e)
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
