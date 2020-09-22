#pragma once

#include "DE_API.h"
#include "Vector.h"

namespace DeltaEngine
{
	struct DE_API Color
	{
		float r, g, b, a;

		static const Color red();
		static const Color green();
		static const Color blue();
		static const Color yellow();
		static const Color magenta();
		static const Color cyan();
		static const Color white();
		static const Color gray();
		static const Color black();
		static const Color clear();

		// default constructor sets the color to white
		Color();
		// copy a predefined color (either static or user declared)
		Color(const Color& copy);
		// custom color
		Color(float r, float b, float g, float a = 1);

		Color& operator=(const Color& a);
		Color operator+(const Color a) const;
		Color operator+=(const Color a);
		Color operator-(const Color a) const;
		Color operator-=(const Color a);
		Color operator*(const float a) const;
		Color operator*=(const float a);

		static Color Lerp(Color a, Color b, float t);
	};
}
