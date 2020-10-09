#include "Math.h"
#include <algorithm>
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
		int RoundDown(float value)
		{
			return static_cast<int>(value);
		}
		int RoundDownToNearest(float value, int multiple)
		{
			return static_cast<int>(value) - static_cast<int>(value) % multiple;
		}
		float RoundDownf(float value)
		{
			return static_cast<float>(static_cast<int>(value));
		}
		float RoundDownToNearestf(float value, int multiple)
		{
			return static_cast<float>(static_cast<int>(value) - static_cast<int>(value) % multiple);
		}
		bool solveQuadratic(const float& a, const float& b, const float& c, float& x0, float& x1)
		{
			float discr = b * b - 4 * a * c;
			if (discr < 0) return false;
			else if (discr == 0) x0 = x1 = -0.5 * b / a;
			else {
				float q = (b > 0) ?
					-0.5f * (b + sqrt1(discr)) :
					-0.5f * (b - sqrt1(discr));
				x0 = q / a;
				x1 = c / q;
			}
			if (x0 > x1) std::swap(x0, x1);

			return true;
		}
		float sqrt1(const float x)
		{
			union
			{
				int i;
				float x;
			} u;
			u.x = x;
			u.i = (1 << 29) + (u.i >> 1) - (1 << 22);

			u.x = u.x + x / u.x;
			u.x = 0.25f * u.x + x / u.x;

			return u.x;
		}
	}
}
