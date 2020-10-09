#include "InputSystem.h"
#include "KeyboardInput.h"
#include "MouseInput.h"

namespace DeltaEngine
{

    InputSystem::InputSystem()
    {

    }

    InputSystem::~InputSystem()
    {

    }

    void InputSystem::reset()
    {
        _keyboard.reset();
        _mouse.reset();
    }

    void InputSystem::update()
    {
        _keyboard.update();
        _mouse.update();
    }

    //void InputSystem::addListener(InputListener* listener)
    //{
    //	_listenerSet.insert(listener);
    //}
    //
    //void InputSystem::removeListener(InputListener* listener)
    //{
    //	_listenerSet.erase(listener);
    //}

    InputSystem* InputSystem::get() // might change in the wei lai
    {
        // returns the address of the InputSystem object
        static InputSystem _inputSystem;

        return &_inputSystem;
    }

    bool InputSystem::isKeyTriggered(int key)
    {
        return _keyboard._isTriggered[key];
    }

    bool InputSystem::isKeyPressed(int key)
    {
        return _keyboard._isPressed[key];
    }

    bool InputSystem::isKeyReleased(int key)
    {
        return _keyboard._isReleased[key];
    }

    bool InputSystem::getShowLine()
    {
        return _keyboard._showLine;
    }

    void InputSystem::setShowLine(bool showLine)
    {
        _keyboard._showLine = showLine;
    }

    bool InputSystem::onMouseMove()
    {
        return _mouse._moveEvent;
    }

    bool InputSystem::isMouseTriggered(int key)
    {
        return _mouse._isTriggered[key];
    }

    bool InputSystem::isMousePressed(int key)
    {
        return _mouse._isPressed[key];
    }

    bool InputSystem::isMouseReleased(int key)
    {
        return _mouse._isReleased[key];
    }

}