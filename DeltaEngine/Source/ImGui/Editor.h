#pragma once

#include <vector>
#include <memory>
#include <string>


#include "Components/Transform.h"
#include "Core/Containers/Singleton.h"
#include "examples/imgui_impl_win32.h"

namespace DeltaEngine
{
  struct Event;
  class EntityManager;
  class IPanel;

  class Editor : public Singleton<Editor>
  {

    friend class Singleton<Editor>;

    void MenuBar();
    Editor();
  public:

    std::vector<std::unique_ptr<IPanel>> m_panels;

    // Tool toggle
    enum class Tool : unsigned
    {
      None = 0,
      Camera,
      EntitySelector
    };

    // Drop Manager
    static inline bool drag;

    // Custom Font Awesome Management, can consider opting DIP
    static inline ImFont* font_awesome;

    // Can consider opting for DIP
    static inline Tool tool_selection;

    // Can consider DIP & include custom debug draw system;
    static inline bool entity_selected;
    static inline size_t entity_id;
    static inline Transform selection_transform;

    // Can consider DIP Simulation toggle To be implemented
    static inline bool simulation_running;

    // Selected Texture
    std::string textureKey {};



    ~Editor();
    void Begin();
    void Render();
    void End();

    static void OnDragDrop(Event* e);
    static void OnRemovingDragDrop(Event* e);
    static void OnDragDropDone(Event* e);
  };
}
