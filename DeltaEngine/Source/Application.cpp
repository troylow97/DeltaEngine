#include "DeltaEngine.h"
#include "Application.h"

#include "EngineConfig.h"
#include "Render/OpenGLSystem.h"
#include "Core/Utils/FileUtils.h"
#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"
#include "Physics/CollisionSystem.h"
#include "ECS/World.h"
#include "Input/InputManager.h"
#include "Audio/AudioEngine.h"
#include "ImGui/Editor.h"
#include "Core/Debugging/Profiler/Profiler.h"
#include "Core/Utils/Random.h"
/*-----------------------------------
#include "Event/ApplicationEvent.h"
#include "Log.h"
-----------------------------------*/

namespace DeltaEngine
{
  DeltaEngineGlobalEnvironment env;
  int fps;

  Application::Application() : m_Minimized{true}, m_interval(0.25)
  {
    Log::Init();
    DeltaEngine_CORE_INFO("Engine Start");
    FileUtils::Root("Assets");

    // Load Engine Configuration
    JsonFile f;
    EngineConfig c;
    f.StartReader("config.json").LoadObject(c).EndReader();
    AudioEngine::Initialize();
    env.pClock = new GameClock(c.fps);

    env.pWin = new Window(c.win_name, c.width, c.height, c.fullscreen);
    env.pWin->Init();

    // Render + Imgui
    RenderModule::openGLSystem = new RenderModule::OpenGLSystem();
    RenderModule::openGLSystem->Init();

    // Randomizer
    Random::Init();

    // Asset Loading
    env.pManager = new AM();
    env.pManager->SetLoader<Font>(new FontLoader()).Load<Font>()
       .SetFallback<Font>(new Font("Fonts/Arial.ttf"));

    env.pManager->SetLoader<Shader>(new ShaderLoader()).Load<Shader>()
       .SetFallback<Shader>(new Shader("Shaders/ErrorShader"));

    env.pManager->SetLoader<Texture2D>(new TextureLoader()).Load<Texture2D>();

    env.pManager->SetLoader<AnimationClip>(new AnimationClipLoader()).Load<AnimationClip>();

    env.pManager->SetLoader<AnimationController>(new AnimationControllerLoader()).Load<AnimationController>();

#ifdef DE_EDITOR
    m_Editor = new Editor();
#endif

    env.eventManager = new EventManager;

    env.pECS = new ECSModule();
  }

  Application::~Application()
  {
    DeltaEngine_CORE_INFO("Engine Shutdown");
    env.pECS->GetWorld().ShutdownSystems();
    delete env.pECS;
    delete env.eventManager;
    delete env.pManager;
    RenderModule::openGLSystem->Exit();
    delete RenderModule::openGLSystem;
#ifdef DE_EDITOR
    delete m_Editor;
#endif
    delete env.pWin;
    delete env.pClock;

    AudioEngine::Shutdown();
  }


  void Application::Run()
  {
    while (env.pWin->Running())
    {
      Profiler::Instance().FrameStart();
      env.pClock->Update();
      InputManager::Instance().Update();
      env.pECS->GetWorld().Update();
#ifdef DE_EDITOR
      m_Editor->Begin();
      m_Editor->Render();
      m_Editor->End();
#endif
      SwapBuffers(RenderModule::openGLSystem->GetWindowContext());
      Profiler::Instance().Record("Buffer Swap");
      OnEvent();
      env.pWin->Update();
      Profiler::Instance().FrameEnd();
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
      delete ref;
    }
    Profiler::Instance().Record("Event");
  }


  bool Application::OnWindowResize(WindowResizeEvent& e)
  {
    if (e.GetWidth() == 0 || e.GetHeight() == 0)
    {
      m_Minimized = true;
      return false;
    }

    m_Minimized = false;

    return false;
  }
}
