#include "DEpch.h"
#include "DeltaEngine.h"
#include "Render/OpenGLSystem.h"
#include "Physics/Collision.h"
#include <typeindex>
#include <typeinfo>
/*-----------------------------------
#include "Event/ApplicationEvent.h"
#include "Log.h"
-----------------------------------*/
namespace DeltaEngine
{

    std::unique_ptr<ECSModule> ecs{ nullptr };

    Application::Application() : m_interval(0.25), m_Running(true), m_Minimized(false)
    {
        DeltaEngine::Log::Init();
        DeltaEngine_CORE_INFO("Engine Start");
        ecs = std::make_unique<ECSModule>();
        // Memory Manager
        // Window
        // Render
        RenderModule::openGLSystem = new RenderModule::OpenGLSystem();
        RenderModule::openGLSystem->Init();
        // GUI
        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
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

        SpriteRenderer* s = new SpriteRenderer();
        TextRenderer* t = new TextRenderer();
        ParticleSystem* p = new ParticleSystem();

        // TODO Modules Instantiation
        f64 accumulator = 0.0;
        bool isRunning = true;
        MSG msg = {};
        while (isRunning)
        {
            Application::OnEvent();
            m_gameclock.Update(); // Update engine GameClock

            accumulator += m_gameclock.DeltaTime(); // Update accumulator using time-scaled dt

            while (accumulator >= m_interval) // Update based on interval
            {
                if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
                {
                    if (msg.message == WM_QUIT)
                        isRunning = false;
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                    continue;
                }
                FixedUpdate();
                VariableUpdate();
                accumulator -= m_gameclock.DeltaTime();
            }
            const f64 alpha = accumulator / m_interval;
        }
        delete p;
        delete t;
        delete s;
        RenderModule::openGLSystem->Exit();
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

    void Application::OnEvent()
    {
        //if (!event_manager.isEmpty())
        //{
        //    auto& ref = event_manager.resolveEvent();
        //
        //    EventDispatcher d(ref);
        //    d.Dispatch<WindowCloseEvent>(DE_BIND_EVENT_FN(Application::OnWindowClose));
        //}

        //below is how cherno does it
        WindowCloseEvent e;
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(DE_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(DE_BIND_EVENT_FN(Application::OnWindowResize));

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
        std::cout << "Window Closing" << std::endl;
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
        RenderModule::openGLSystem->Update();
        // GUI Update
        // Memory Update
    }
}
