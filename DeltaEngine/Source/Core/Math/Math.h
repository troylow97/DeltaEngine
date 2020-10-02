#pragma once

namespace DeltaEngine
{
	namespace Math
	{
		const float pi = 3.14159265358979323846f;
		
		int Clamp(int value, int min, int max);
		float Clamp(float value, float min, float max);
		float Clamp01(float value);
		float Lerp(float a, float b, float t);
		int RoundDown(float value);
		int RoundDownToNearest(float value, int multiple);
		float RoundDownf(float value);
		float RoundDownToNearestf(float value, int multiple);
	}
}
