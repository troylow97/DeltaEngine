#pragma once

#include <unordered_map>

namespace DeltaEngine
{
  class KeyboardInput
  {
    bool m_is_triggered[256]{};
    bool m_is_pressed[256]{};
    bool m_is_released[256]{};
    unsigned char m_current_key_state[256] = {}; // an array of key states
    unsigned char m_previous_key_state[256] = {}; // an array of old key states

    friend class InputManager;

    void Reset();
    void Update();
  };
}
