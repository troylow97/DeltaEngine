#include "InputSystem.h"

namespace DeltaEngine::Input
{
	InputSystem::InputSystem()
	{

	}

	InputSystem::~InputSystem()
	{
		
	}

	void InputSystem::inputReset()
	{
		_keyboard.reset();
		_mouse.reset();
	}

	void InputSystem::inputUpdate()
	{
		_keyboard.keyboardUpdate();
		_mouse.mouseUpdate();
		/////////////////////////////////////////////////////////////
		//POINT _currentMousePosition = {};
		//::GetCursorPos(&_currentMousePosition); 
		//
		//if (_mouseFirstTime)
		//{
		//	// initialize old position attributes with the current position attributes
		//	_previousMousePosition = Point(_currentMousePosition.x, _currentMousePosition.y);
		//	_mouseFirstTime = false; 
		//}
		//
		//if (_currentMousePosition.x != _previousMousePosition.point_x || _currentMousePosition.y != _previousMousePosition.point_y)
		//{
		//	// there is a mouse event so input listener has to be notified 
		//	std::unordered_set<InputListener*>::iterator it = _listenerSet.begin();
		//
		//	while (it != _listenerSet.end())
		//	{
		//		/* -------------------------------------------------------------------------------------------------------------------------------------------------------
		//		* spencey chan says keep this !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//		* can create a function so others can link, maybe use bind, bind a function with mouse move, which by default does not have any function binded to it
		//		* so to use mouse move i need to bind a function first 
		//		* if not binded do what? if binded do what? 
		//		* need keep track of prev and current, use an API to keep track of those (2d game not so important but good to have) 
		//		------------------------------------------------------------------------------------------------------------------------------------------------------- */
		//		(*it)->onMouseMove(Point(_currentMousePosition.x /*- _previousMousePosition.point_x*/, _currentMousePosition.y /*- _previousMousePosition.point_y*/));
		//		++it;
		//	}
		//}
		//_previousMousePosition = Point(_currentMousePosition.x, _currentMousePosition.y);
		//
		//// ---------------------------------------------------------------------------------------------------------------------------------------------------------
		//
		//// allows us to retrieve the entire sequence of state of each key of the keyboard
		//if (::GetKeyboardState(_currentKeyState)) 
		//{
		//	// checks the state of each key, checks if key is pressed (or not)
		//	for (unsigned int i = 0; i < 256; ++i) 
		//	{
		//		// bitmasking, only the higher bits of the value are being evaluated 
		//		// key is pressed if the value is 1 or 2
		//		if (_currentKeyState[i] & 0x80) // 0x80 = key is pressed or not, this is like the event, so if true, do the capturing of the snapshot
		//		// event is like a nice way of wrapping something, like a message "this is a key pressed" "this is your captain speaking"
		//		{
		//			// loop the unordered_set of listener and notify each listener that it is being pressed by calling isKeyPressed()
		//			// iterator is the function i tied back, so it will take every single key and check with isKeyPressed()
		//			std::unordered_set<InputListener*>::iterator it = _listenerSet.begin(); 
		//
		//			if (_currentKeyState[i] != _previousKeyState[i])
		//			{
		//				while (it != _listenerSet.end())
		//				{
		//					if (i == DEVK_LBUTTON)
		//					{
		//						// event should only be called once, not everytime when inputUpdate() is being called 
		//						(*it)->isLeftMouseTriggered(Point(_currentMousePosition.x, _currentMousePosition.y));
		//					}
		//					else if (i == DEVK_RBUTTON)
		//					{
		//						// event should only be called once, not everytime when inputUpdate() is being called 
		//						(*it)->isRightMouseTriggered(Point(_currentMousePosition.x, _currentMousePosition.y));
		//					}
		//					else
		//					{
		//						(*it)->isKeyTriggered(i);
		//					}
		//					++it;
		//				}
		//			}
		//			else
		//			{
		//				while (it != _listenerSet.end())
		//				{
		//					if (i == DEVK_LBUTTON)
		//					{
		//						// event should only be called once, not everytime when inputUpdate() is being called 
		//						(*it)->isLeftMousePressed(Point(_currentMousePosition.x, _currentMousePosition.y));
		//					}
		//					else if (i == DEVK_RBUTTON)
		//					{
		//						// event should only be called once, not everytime when inputUpdate() is being called 
		//						(*it)->isRightMousePressed(Point(_currentMousePosition.x, _currentMousePosition.y));
		//					}
		//					else
		//					{
		//						(*it)->isKeyPressed(i);
		//					}
		//					++it;
		//				}
		//			}
		//
		//		}
		//		// key is not pressed
		//		else
		//		{
		//			// checks if key is not pressed
		//			// if the current key states != previous key states 
		//			if (_currentKeyState[i] != _previousKeyState[i])
		//			{
		//				// loop through listener to notify them of isKeyReleased() event
		//				std::unordered_set<InputListener*>::iterator it = _listenerSet.begin();
		//
		//				while (it != _listenerSet.end())
		//				{
		//					if (i == DEVK_LBUTTON)
		//					{
		//						(*it)->isLeftMouseReleased(Point(_currentMousePosition.x, _currentMousePosition.y));
		//					}
		//					else if (i == DEVK_RBUTTON)
		//					{
		//						(*it)->isRightMouseReleased(Point(_currentMousePosition.x, _currentMousePosition.y));
		//					}
		//					else
		//					{
		//						(*it)->isKeyReleased(i);
		//					}
		//					++it;
		//				}
		//			}
		//		}
		//	}
		//	// stores the current key states to previous key states
		//	::memcpy(_previousKeyState, _currentKeyState, (sizeof(unsigned char) * 256));
		//}
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