/**********************************************************************************
* \file   KeyboardInput.h
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
