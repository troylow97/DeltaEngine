#pragma once

#include "Core/Math/Point.h"
#include <unordered_map>

namespace DeltaEngine
{
  class MouseInput
  {
  private:
#pragma warning(disable:4251)
    std::unordered_map<unsigned int, bool> m_is_triggered;
    std::unordered_map<unsigned int, bool> m_is_pressed;
    std::unordered_map<unsigned int, bool> m_is_released;
#pragma warning(default:4251)

    unsigned char m_current_key_state[256] = {}; // an array of key states
    unsigned char m_previous_key_state[256] = {}; // an array of old key states
    Point m_current_mouse_position; // the coordinate of current mouse position 
    Point m_previous_mouse_position; // the coordinate of previous mouse position 
    Point m_current_camera_mouse_position; // the coordinate of current mouse position in camera viewport
    Point m_previous_camera_mouse_position; // the coordinate of previous mouse position in camera viewport
    bool m_move_event;
    bool m_first_time = true;

    bool m_entity_selected = false;
    size_t m_entityid_selected;
    bool m_entity_dragged = false;
    bool m_entity_allow_drag = false;
    bool m_camera_dragged = false;
    bool m_mouse_in_viewport = false;
    bool m_entity_chosen_in_viewport = false;
    bool m_entity_chosen_in_world = false;

    bool m_tileset_dragged = false;
    bool m_asset_dragged = false;

    friend class InputManager;

    void Reset();
    void Update();

  public:
    bool IsMouseTriggered(int key);
    bool IsMousePressed(int key);
    bool IsMouseReleased(int key);
    bool IsMouseMoved();
    const Point& CurrentPosition(); // getter 
    const Point& PreviousPosition(); // getter
    const Point& CurrentCameraPosition(); // getter
    const Point& PreviousCameraPosition(); // getter
    void SetCurrentPosition(Point _currentPosition); // setter
    void SetPreviousPosition(Point _previousPosition); // setter
    void SetCurrentCameraPosition(Point _currentCameraPosition); // setter
    void SetPreviousCameraPosition(Point _previousCameraPosition); // setter

    bool EntitySelected(); // getter
    void SetEntitySelected(bool _setEntitySelected); // setter
    bool EntityDragged(); // getter
    void SetEntityDragged (bool _setEntityDragged); // setter
    bool EntityAllowDrag(); // getter
    void SetEntityAllowDrag(bool _setEntityAllowDragged); // setter
    bool CameraDragged(); // getter
    void SetCameraDragged(bool _setCameraDragged); // setter
    bool MouseInViewPort(); // getter
    void SetMouseInViewPort(bool _setMouseInViewPort); // setter
    bool EntityChosenInViewport(); // getter
    void SetEntityChosenInViewport(bool _setEntityChosenInViewport); // setter
    bool EntityChosenInWorld(); // getter
    void SetEntityChosenInWorld(bool _setEntityChosenInViewport); // setter

    size_t EntityIDSelected(); // getter
    void SetEntityIDSelected(size_t _setEntityIDSelected); // setter

    bool TilesetDragged(); // getter
    void SetTilesetDragged(bool _setTilesetDragged); // setter
  };
}
