#include "Math.h"

namespace DeltaEngine
{
	namespace Math
	{
		int Clamp(int value, int min, int max)
		{
			if (value < min)
				return min;
			if (value > max)
				return max;
			return value;
		}
		float Clamp(float value, float min, float max)
		{
			if (value < min)
				return min;
			if (value > max)
				return max;
			return value;
		}
		float Clamp01(float value)
		{
			if (value < 0)
				return 0;
			if (value > 1)
				return 1;
			return value;
		}
		float Lerp(float a, float b, float t)
		{
			return a + (b - a) * t;
		}
	}
}
