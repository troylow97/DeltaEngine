#pragma once

#include "DE_API.h"
#include "MouseInput.h"
#include "KeyboardInput.h"
#include "Keys.h"

namespace DeltaEngine
{
    class DE_API InputManager //: public InputListener
    {

        KeyboardInput m_keyboard;
        MouseInput m_mouse;

    public:
        InputManager();
        ~InputManager();

        void Reset();
        // to get the input events
        void Update();

        //void addListener(InputListener* listener);
        //void removeListener(InputListener* listener);

        // get static method as InputManager is a singleton class
        // returns a pointer to an only instance of the class
        static InputManager* Get();

        bool IsKeyTriggered(int key);
        bool IsKeyPressed(int key);
        bool IsKeyReleased(int key);
        bool GetShowLine();
        void SetShowLine(bool showLine);

        bool OnMouseMove();

        bool IsMouseTriggered(int key);
        bool IsMousePressed(int key);
        bool IsMouseReleased(int key);
        const Point& CurrentPosition();
        const Point& PreviousPosition();
    };

}