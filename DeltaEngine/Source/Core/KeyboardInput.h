#pragma once

#include "Core/Core.h"

#include <unordered_map>
#include <Windows.h>

namespace DeltaEngine::Input
{
	class DE_API KeyboardInput
	{
	private:
		std::unordered_map<unsigned int, bool> _isTriggered;
		std::unordered_map<unsigned int, bool> _isPressed ;
		std::unordered_map<unsigned int, bool> _isReleased;

		unsigned char _currentKeyState[256] = { };  // an array of key states
		unsigned char _previousKeyState[256] = { }; // an array of old key states

		friend class InputSystem;

		void reset();
		void keyboardUpdate();

	public:
		bool isKeyTriggered(int key);
		bool isKeyPressed(int key);
		bool isKeyReleased(int key);
	};
}