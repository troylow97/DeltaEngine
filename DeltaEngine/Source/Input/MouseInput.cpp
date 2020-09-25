#include "MouseInput.h"
#include <Windows.h>

namespace DeltaEngine
{
	void MouseInput::reset()
	{
		_moveEvent = false;

		for (auto& it : _isTriggered)
		{
			if (it.second)
			{
				_isPressed[it.first] = true;
				it.second = false;
			}
		}
		for (auto& it : _isReleased)
		{
			if (it.second)
			{
				it.second = false;
			}
		}
		// scroll event maybe
	}

	void MouseInput::update()
	{
		POINT _currentPosition = {};
		::GetCursorPos(&_currentPosition);

		// allows us to retrieve the entire sequence of state of each key of the keyboard
		if (::GetKeyboardState(_currentKeyState))
		{
			// checks the state of each key 
			for (unsigned int i = 0; i < 256; ++i)
			{
				// bitmasking, only the higher bits of the value are being evaluated 
				// key is pressed if the value is 1 or 2
				if (_currentKeyState[i] & 0x80)
				{
					// if triggered
					if (_currentKeyState[i] != _previousKeyState[i])
					{
						_isTriggered[i] = true;
						_isPressed[i] = false;
						_isReleased[i] = false;
					}
					// if pressed
					else
					{
						_isTriggered[i] = false;
						_isPressed[i] = true;
						_isReleased[i] = false;
					}
				}
				// if released
				else
				{
					if (_currentKeyState[i] != _previousKeyState[i])
					{
						_isTriggered[i] = false;
						_isPressed[i] = false;
						_isReleased[i] = true;
					}
					else
					{
						_isTriggered[i] = false;
						_isPressed[i] = false;
						_isReleased[i] = false;
					}
				}
			}
			::memcpy(_previousKeyState, _currentKeyState, (sizeof(unsigned char) * 256));
		}
		if (_currentMousePosition.point_x != _previousMousePosition.point_x || _currentMousePosition.point_y != _previousMousePosition.point_y)
		{
			_previousMousePosition = Point(_currentMousePosition.point_x, _currentMousePosition.point_y);
			_currentMousePosition = Point(_currentPosition.x, _currentPosition.y);
			_moveEvent = true;
		}
	}

	bool MouseInput::isMouseTriggered(int key)
	{
		return _isTriggered[key];
	}

	bool MouseInput::isMousePressed(int key)
	{
		return _isPressed[key];
	}

	bool MouseInput::isMouseReleased(int key)
	{
		return _isReleased[key];
	}

	bool MouseInput::isMouseMoved()
	{
		return _moveEvent;
	}

	const Point& MouseInput::currentPosition()
	{
		return _currentMousePosition;
	}
	const Point& MouseInput::previousPosition()
	{
		return _previousMousePosition;
	}
}