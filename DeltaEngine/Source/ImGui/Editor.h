#pragma once

#include <vector>
#include <memory>
#include "examples/imgui_impl_opengl3.h"
#include "examples/imgui_impl_win32.h"

namespace DeltaEngine
{
  struct Event;
  class EntityManager;
  class IPanel;

  class Editor
  {
    std::vector<std::unique_ptr<IPanel>> m_panels;


    inline static bool drag;
    void MenuBar();

  public:
    inline static ImFont* m_font_awesome;

    Editor();
    ~Editor();
    void Begin();
    void Render();
    void End();

    static void OnDragDrop(Event* e);
    static void OnRemovingDragDrop(Event* e);
    static void OnDragDropDone(Event* e);
  };
}
