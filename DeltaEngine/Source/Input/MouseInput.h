#pragma once

#include "DE_API.h"
#include "Core/Math/Point.h"
#include <unordered_map>

namespace DeltaEngine
{
class DE_API MouseInput
{
private:
#pragma warning(disable:4251)
  std::unordered_map<unsigned int, bool> m_is_triggered;
  std::unordered_map<unsigned int, bool> m_is_pressed;
  std::unordered_map<unsigned int, bool> m_is_released;
#pragma warning(default:4251)

  unsigned char m_current_key_state[256] = { };  // an array of key states
  unsigned char m_previous_key_state[256] = { }; // an array of old key states
  Point m_current_mouse_position;                // the coordinate of current mouse position 
  Point m_previous_mouse_position;               // the coordinate of previous mouse position 
  bool m_move_event;
  bool m_first_time = true;

  friend class InputManager;

  void Reset();
  void Update();

public:
  bool IsMouseTriggered( int key );
  bool IsMousePressed( int key );
  bool IsMouseReleased( int key );
  bool IsMouseMoved();
  const Point &CurrentPosition();
  const Point &PreviousPosition();
};
}