#include "InputSystem.h"

namespace DeltaEngine
{
	namespace Input
	{
		//InputSystem::InputSystem()
		//{
		//
		//}

		void InputSystem::left()
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
			{
				std::cout << "AAAAAAAAAA" << std::endl;
				// blah 
				/* ------------------------------------------------
				// if want to set the mouse position based on input
				sf::Mouse::setPosition(sf::Vector2i(0, 0), console);
				------------------------------------------------ */
			}
			sf::Vector2i mousePosition = sf::Mouse::getPosition(/* console */);
			std::cout << mousePosition.x << " : " << mousePosition.y << std::endl;
		}
	}
	
}