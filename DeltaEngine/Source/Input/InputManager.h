/**********************************************************************************
* \file   InputManager.h
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

#include "MouseInput.h"
#include "KeyboardInput.h"
#include "Core/Containers/Singleton.h"

namespace DeltaEngine
{
  class InputManager : public Singleton<InputManager> //: public InputListener
  {
    KeyboardInput m_keyboard;
    MouseInput m_mouse;

    friend class Singleton<InputManager>;
    InputManager();
    ~InputManager();

  public:
    
    void Reset();
    void Update();

    bool IsKeyTriggered(int key);
    bool IsKeyPressed(int key);
    bool IsKeyReleased(int key);

    bool OnMouseMove();

    bool IsMouseTriggered(int key);
    bool IsMousePressed(int key);
    bool IsMouseReleased(int key);
    const Point& CurrentPosition(); // getter 
    const Point& PreviousPosition(); // getter

    bool* GetKeys();
  };
}
