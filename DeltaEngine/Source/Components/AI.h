#pragma once
#include <string>

struct AI
{
	std::string key;	// "guard"
	unsigned int index; //Not used

	AI() :
		key{"idle"},
		index{0}
	{}
};