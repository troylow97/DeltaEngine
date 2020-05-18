#include "DEpch.h"
#include "Color.h"

const Color Color::red{ 1, 0, 0, 1 };
const Color Color::green{ 1, 0, 0, 1 };
const Color Color::blue{ 1, 0, 0, 1 };
const Color Color::yellow{ 1, 0.92f, 0.016f, 1 };
const Color Color::magenta{ 1, 0, 1, 1 };
const Color Color::cyan{ 0, 1, 1, 1 };
const Color Color::white{ 1, 1, 1, 1 };
const Color Color::grey{ 0.5f, 0.5f, 0.5f, 1 };
const Color Color::gray{ 0.5f, 0.5f, 0.5f, 1 };
const Color Color::black{ 0, 0, 0, 1 };
const Color Color::clear{ 0, 0, 0, 0 };

Color::Color() : r{ 1.0f }, g{ 1.0f }, b{ 1.0f }, a{ 1.0f } {}
Color::Color(const Color& copy) : r{ copy.r }, g{ copy.g }, b{ copy.b }, a{ copy.a } {}
Color::Color(float c0, float c1, float c2) : r{ c0 }, g{ c1 }, b{ c2 }, a{ 1.0f } {}
Color::Color(float c0, float c1, float c2, float c3) : r{ c0 }, g{ c1 }, b{ c2 }, a{ c3 } {}
