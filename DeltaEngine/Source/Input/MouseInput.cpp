/**********************************************************************************
* \file   MouseInput.cpp
* \brief  The file contains the logic for mouse input
* \author Chin, Clara,   100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "MouseInput.h"
#include <Windows.h>

namespace DeltaEngine
{
  void MouseInput::Reset()
  {
    m_move_event = false;

    for (auto& it : m_is_triggered)
    {
      if (it.second)
      {
        m_is_pressed[it.first] = true;
        it.second = false;
      }
    }
    for (auto& it : m_is_released)
    {
      if (it.second)
      {
        it.second = false;
      }
    }
    // scroll event maybe
  }

  void MouseInput::Update()
  {
    POINT _currentPosition = {};
    GetCursorPos(&_currentPosition);

    // allows us to retrieve the entire sequence of state of each key of the keyboard
    if (GetKeyboardState(m_current_key_state))
    {
      // checks the state of each key 
      for (unsigned int i = 0; i < 256; ++i)
      {
        // bitmasking, only the higher bits of the value are being evaluated 
        // key is pressed if the value is 1 or 2
        if (m_current_key_state[i] & 0x80)
        {
          // if triggered
          if (m_current_key_state[i] != m_previous_key_state[i])
          {
            m_is_triggered[i] = true;
            m_is_pressed[i] = false;
            m_is_released[i] = false;
          }
            // if pressed
          else
          {
            m_is_triggered[i] = false;
            m_is_pressed[i] = true;
            m_is_released[i] = false;
          }
        }
          // if released
        else
        {
          if (m_current_key_state[i] != m_previous_key_state[i])
          {
            m_is_triggered[i] = false;
            m_is_pressed[i] = false;
            m_is_released[i] = true;
          }
          else
          {
            m_is_triggered[i] = false;
            m_is_pressed[i] = false;
            m_is_released[i] = false;
          }
        }
      }
      memcpy(m_previous_key_state, m_current_key_state, (sizeof(unsigned char) * 256));
    }
    if (_currentPosition.x != m_previous_mouse_position.point_x || _currentPosition.y != m_previous_mouse_position.
      point_y)
    {
      m_current_mouse_position = Point(m_previous_mouse_position.point_x, m_previous_mouse_position.point_y);
      m_previous_mouse_position = Point(static_cast<float>(_currentPosition.x), static_cast<float>(_currentPosition.y));
      m_move_event = true;
    }
  }
}
