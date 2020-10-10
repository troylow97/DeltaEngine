#pragma once

#include "DE_API.h"

#include <unordered_map>

namespace DeltaEngine
{
	class DE_API KeyboardInput
	{
	private:
#pragma warning(disable:4251)
		std::unordered_map<unsigned int, bool> m_is_triggered;
		std::unordered_map<unsigned int, bool> m_is_pressed;
		std::unordered_map<unsigned int, bool> m_is_released;
#pragma warning(default:4251)
		unsigned char m_current_key_state[256] = { };  // an array of key states
		unsigned char m_previous_key_state[256] = { }; // an array of old key states

		friend class InputManager;

		bool m_show_line = false;

		void Reset();
		void Update();

	public:
		bool IsKeyTriggered(int key);
		bool IsKeyPressed(int key);
		bool IsKeyReleased(int key);
		bool GetShowLine();
		void SetShowLine(bool showLine);
	};
}