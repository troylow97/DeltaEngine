#pragma once

#include "DE_API.h"

#include <unordered_map>

namespace DeltaEngine
{
	class DE_API KeyboardInput
	{
	private:
		std::unordered_map<unsigned int, bool> _isTriggered;
		std::unordered_map<unsigned int, bool> _isPressed;
		std::unordered_map<unsigned int, bool> _isReleased;

		unsigned char _currentKeyState[256] = { };  // an array of key states
		unsigned char _previousKeyState[256] = { }; // an array of old key states

		friend class InputSystem;

		bool _showLine = false;

		void reset();
		void update();

	public:
		bool isKeyTriggered(int key);
		bool isKeyPressed(int key);
		bool isKeyReleased(int key);
		bool getShowLine();
		void setShowLine(bool showLine);
	};
}