#pragma once
#include "Components.h"

namespace DeltaEngine
{

struct Input
{
	int previousKey;
	int currentKey;
	REGISTER_COMPONENT(Input)
};

}