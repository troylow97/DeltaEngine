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
    bool GetShowLine();
    void SetShowLine(bool showLine);

    bool OnMouseMove();

    bool IsMouseTriggered(int key);
    bool IsMousePressed(int key);
    bool IsMouseReleased(int key);
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
    void SetEntityDragged(bool _setEntityDragged); // setter
    bool CameraDragged(); // getter
    void SetCameraDragged(bool _setCameraDragged); // setter
    bool MouseInViewPort(); // getter
    void SetMouseInViewPort(bool _setMouseInViewPort); // setter

    size_t EntityIDSelected(); // getter
    void SetEntityIDSelected(size_t _setEntityIDSelected); // setter

    bool TilesetDragged(); // getter
    void SetTilesetDragged(bool _setTilesetDragged); // setter

    bool* GetKeys();
  };
}
