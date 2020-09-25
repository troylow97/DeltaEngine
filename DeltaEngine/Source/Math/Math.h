#pragma once
#include "Vector.h"
#include "Matrix.h"
namespace DeltaEngine
{
	namespace Math
	{
		int Clamp(int value, int min, int max);
		float Clamp(float value, float min, float max);
		float Clamp01(float value);
	}
}
