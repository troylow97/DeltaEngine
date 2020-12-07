/**********************************************************************************
* \file			Random.cpp
* \brief		Functions for generating random numbers.
* \author		Low Yee Troy, 100% Code Contribution
* \version		1.0
* \date			2020
*
* \note			Course: GAM150
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include <chrono>
#include <random>

class Random
{
public:
  /**************************************************************************/
  /*!
    \brief	Call this function at the start of the game.
  */
  /**************************************************************************/
  static void Init()
  {
    unsigned seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
    s_RandomEngine.seed(seed);
  }

  /**************************************************************************/
  /*!
    \brief	Returns a float between 0 and 1
  */
  /**************************************************************************/
  static float RandomFloat()
  {
    static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    return static_cast<float>(distribution(s_RandomEngine));
  }

  /**************************************************************************/
  /*!
    \brief	Returns an int between a range.
            Example min = 0, max = 3. Numbers given would be 0,1,2
  
    \param	min: mininum number in the range
  
    \param	max: maximum number in the range
  */
  /**************************************************************************/
  static int RandomIntRange(int min, int max)
  {
    return static_cast<int>(min + (RandomFloat() * (max - min)));
  }

  /**************************************************************************/
  /*!
    \brief	Returns an float between a range
  
    \param	min: mininum number in the range
  
    \param	max: maximum number in the range
  */
  /**************************************************************************/
  static float RandomFloatRange(float min, float max)
  {
    return (min + (RandomFloat() * (max - min)));
  }

private:
  //Mersenne Twister 19937 generator
  static std::mt19937 s_RandomEngine;
  static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};
