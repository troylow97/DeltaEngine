#pragma once
#include <string>

struct AI
{
	std::string key;	// "guard"
	unsigned int index;

	AI() :
		key{"idle"},
		index{0}
	{}
};