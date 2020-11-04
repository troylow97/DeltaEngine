#pragma once
#include "Core/LayerStack.h"
#include "Event/ApplicationEvent.h"
#include "DE_API.h"
#include "Core/Debugging/Logger/Log.h"
#include "ImGui/ImGuiLayer.h"
#include <memory>
#include "ImGui/Editor.h"
namespace DeltaEngine
{

class DE_API Application
{
  bool m_Minimized;

  double m_interval;

  ImGuiLayer *m_ImGuiLayer;
  Editor* m_Editor;
public:
  Application();
  virtual ~Application();

  void Run();
  void OnEvent();
  bool OnWindowResize( WindowResizeEvent &e );

};

Application *CreateApplication();
}
