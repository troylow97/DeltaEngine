#pragma once

#include "DE_API.h"
#include "MouseInput.h"
#include "KeyboardInput.h"
#include "Keys.h"

namespace DeltaEngine
{
    class DE_API InputManager //: public InputListener
    {

        KeyboardInput _keyboard;
        MouseInput _mouse;

    public:
        InputManager();
        ~InputManager();

        void reset();
        // to get the input events
        void update();

        //void addListener(InputListener* listener);
        //void removeListener(InputListener* listener);

        // get static method as InputManager is a singleton class
        // returns a pointer to an only instance of the class
        static InputManager* get();

        bool isKeyTriggered(int key);
        bool isKeyPressed(int key);
        bool isKeyReleased(int key);
        bool getShowLine();
        void setShowLine(bool showLine);

        bool onMouseMove();

        bool isMouseTriggered(int key);
        bool isMousePressed(int key);
        bool isMouseReleased(int key);
        const Point& currentPosition();
        const Point& previousPosition();
    };

}