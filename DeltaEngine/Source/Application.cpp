#include "DEpch.h"
#include "DeltaEngine.h"
#include "Application.h"
#include "Render/OpenGLSystem.h"
#include "Event/ApplicationEvent.h"
#include "Physics/Collision.h"

/*-----------------------------------
#include "Event/ApplicationEvent.h"
#include "Log.h"
-----------------------------------*/
namespace DeltaEngine
{
    Application::Application() : m_interval(0.25), m_Running(true), m_Minimized(false)
    {
        DeltaEngine::Log::Init();
        DeltaEngine_CORE_INFO("Engine Start");
        // Memory Manager
        // Window
        // Render
        // GUI
        // Physics
        // Audio
        // Events
    }

    Application::~Application()
    {
        DeltaEngine_CORE_INFO("Engine Shutdown");
        // Events
        // Audio
        // Physics
        // GUI
        // Render
        // Window
        // Memory Manager
    }

    void Application::Run()
    {
        m_Running = true;
        // a lot of this should be moved to a function in GraphicsManager later
        RenderModule::openGLSystem = new RenderModule::OpenGLSystem();
        RenderModule::openGLSystem->Init();

        SpriteRenderer* s = new SpriteRenderer();
        TextRenderer* t = new TextRenderer();
        ParticleSystem* p = new ParticleSystem();

        // TODO Modules Instantiation
        f64 accumulator = 0.0;

        MSG msg = {};
        while (msg.message != WM_QUIT)
        {
            m_gameclock.Update(); // Update engine GameClock

            accumulator += m_gameclock.DeltaTime(); // Update accumulator using time-scaled dt

            while (accumulator >= m_interval) // Update based on interval
            {
                if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                    continue;
                }
                RenderModule::openGLSystem->Update();
                FixedUpdate();
                accumulator -= m_gameclock.DeltaTime();
            }
            const f64 alpha = accumulator / m_interval;
        }
        RenderModule::openGLSystem->Exit();
        delete s;
        delete t;
        delete p;
        delete RenderModule::openGLSystem;
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
        // GUI Update
        // Memory Update
    }
}
