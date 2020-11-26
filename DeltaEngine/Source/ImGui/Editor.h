#pragma once

#include <vector>
#include <memory>


#include "Components/Transform.h"
#include "examples/imgui_impl_win32.h"

namespace DeltaEngine
{
  struct Event;
  class EntityManager;
  class IPanel;

  class Editor
  {

    std::vector<std::unique_ptr<IPanel>> m_panels;


    void MenuBar();

  public:
    // Drop Manager
    inline static bool drag;

    // Custom Font Awesome Management
    inline static ImFont* font_awesome;

    // Tool toggle
    enum class Tool : unsigned
    {
      None = 0,
      Camera,
      EntitySelector
    };

    inline static Tool tool_selection;

    inline static bool entity_selected;
    inline static size_t entity_id;

    inline static Transform selection_transform;

    // Simulation toggle
    inline static bool simulation_running;


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
