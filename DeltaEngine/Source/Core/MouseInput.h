#pragma once

#include "Core/Core.h"
#include "Math/Point.h"

#include <unordered_map>
#include <Windows.h>

namespace DeltaEngine::Input
{
	class DE_API MouseInput
	{
	private:
		std::unordered_map<unsigned int, bool> _isTriggered;
		std::unordered_map<unsigned int, bool> _isPressed;
		std::unordered_map<unsigned int, bool> _isReleased;

		unsigned char _currentKeyState[256] = { };  // an array of key states
		unsigned char _previousKeyState[256] = { }; // an array of old key states
		Point _currentMousePosition;                // the coordinate of current mouse position 
		Point _previousMousePosition;               // the coordinate of previous mouse position 
		bool _moveEvent;

		friend class InputSystem;

		void reset();
		void mouseUpdate();

	public:
		bool isMouseTriggered(int key);
		bool isMousePressed(int key);
		bool isMouseReleased(int key);
		bool isMouseMoved();
		const Point& currentPosition();
		const Point& previousPosition();
	};
}