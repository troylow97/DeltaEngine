/**********************************************************************************
* \file			Random.cpp
* \brief		Functions for creating Random Numbers
* \author		Low Yee Troy, 100% Code Contribution
* \version		1.0
* \date			2020
*
* \note			Course: GAM150
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "DEpch.h"
#include "Random.h"

std::mt19937 Random::s_RandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;