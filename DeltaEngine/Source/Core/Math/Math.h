/**********************************************************************************
* \file   Math.h
* \brief  The file contains BLAHBLAHBLAH
* \author Chin, Clara,   X% Code Contribution
* \author Low, Troy,     X% Code Contribution
* \author Ong, Graeme,   X% Code Contribution
* \author Tan, Tong Wee, X% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "Vector.h"
#include "Matrix.h"

namespace DeltaEngine::Math
{
  const float pi = 3.14159265358979323846f;

  template <typename T>
  const T& MathMax(const T& a, const T& b)
  {
    return (a < b) ? b : a;
  }

  template <typename T>
  const T& MathMin(const T& a, const T& b)
  {
    return (a < b) ? a : b;
  }

  int Clamp(int value, int min, int max);
  float Clamp(float value, float min, float max);
  float Clamp01(float value);
  float Lerp(float a, float b, float t);
  int RoundDown(float value);
  int RoundDownToNearest(float value, int multiple);
  float RoundDownf(float value);
  float RoundDownToNearestf(float value, int multiple);
  int Abs(int value);
  float Abs(float value);
}
