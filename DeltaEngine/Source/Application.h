#pragma once
#include "Event/ApplicationEvent.h"
#include "DE_API.h"
#include "ImGui/Editor.h"
namespace DeltaEngine
{

class DE_API Application
{
  bool m_Minimized;

  double m_interval;

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
