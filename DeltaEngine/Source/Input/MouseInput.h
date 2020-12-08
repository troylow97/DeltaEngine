/**********************************************************************************
* \file   MouseInput.h
* \brief  The file contains BLAHBLAHBLAH
* \author Chin, Clara,   X% Code Contribution
* \author Low, Troy,     X% Code Contribution
* \author Ong, Graeme,   X% Code Contribution
* \author Tan, Tong Wee, X% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "Core/Math/Point.h"
#include <unordered_map>

namespace DeltaEngine
{
  class MouseInput
  {
    std::unordered_map<unsigned int, bool> m_is_triggered;
    std::unordered_map<unsigned int, bool> m_is_pressed;
    std::unordered_map<unsigned int, bool> m_is_released;

    unsigned char m_current_key_state[256] = {}; // an array of key states
    unsigned char m_previous_key_state[256] = {}; // an array of old key states
    Point m_current_mouse_position; // the coordinate of current mouse position 
    Point m_previous_mouse_position; // the coordinate of previous mouse position 
    bool m_move_event {false};
    friend class InputManager;

    void Reset();
    void Update();
  };
}
