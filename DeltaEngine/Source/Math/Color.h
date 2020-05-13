#pragma once

#include "Vector.h"

struct Color
{
	float r, g, b, a;

	static Color red, green, blue, yellow, magenta, cyan, white, grey, gray, black, clear;

	// default constructor sets the color to white
	Color();
	// copy a predefined color (either static or user declared)
	Color(const Color& copy);
	// assumes alpha is 1
	Color(float r, float b, float g);
	// custom color
	Color(float r, float b, float g, float a);
};
