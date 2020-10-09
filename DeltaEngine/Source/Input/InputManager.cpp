#include "InputManager.h"
#include "KeyboardInput.h"
#include "MouseInput.h"

namespace DeltaEngine
{

    InputManager::InputManager()
    {

    }

    InputManager::~InputManager()
    {

    }

    void InputManager::reset()
    {
        _keyboard.reset();
        _mouse.reset();
    }

    void InputManager::update()
    {
        _keyboard.update();
        _mouse.update();
    }

    //void InputManager::addListener(InputListener* listener)
    //{
    //	_listenerSet.insert(listener);
    //}
    //
    //void InputManager::removeListener(InputListener* listener)
    //{
    //	_listenerSet.erase(listener);
    //}

    InputManager* InputManager::get() // might change in the wei lai
    {
        // returns the address of the InputManager object
        static InputManager _InputManager;

        return &_InputManager;
    }

    bool InputManager::isKeyTriggered(int key)
    {
        return _keyboard._isTriggered[key];
    }

    bool InputManager::isKeyPressed(int key)
    {
        return _keyboard._isPressed[key];
    }

    bool InputManager::isKeyReleased(int key)
    {
        return _keyboard._isReleased[key];
    }

    bool InputManager::getShowLine()
    {
        return _keyboard._showLine;
    }

    void InputManager::setShowLine(bool showLine)
    {
        _keyboard._showLine = showLine;
    }

    bool InputManager::onMouseMove()
    {
        return _mouse._moveEvent;
    }

    bool InputManager::isMouseTriggered(int key)
    {
        return _mouse._isTriggered[key];
    }

    bool InputManager::isMousePressed(int key)
    {
        return _mouse._isPressed[key];
    }

    bool InputManager::isMouseReleased(int key)
    {
        return _mouse._isReleased[key];
    }

    const Point& InputManager::currentPosition()
    {
        return _mouse._currentMousePosition;
    }

    const Point& InputManager::previousPosition()
    {
        return _mouse._previousMousePosition;
    }
}