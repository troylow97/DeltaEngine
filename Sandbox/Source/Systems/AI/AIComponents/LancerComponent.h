#pragma once
#include "Core/Math/Vector.h"

struct Lancer
{
    DeltaEngine::Vector2 ChargeDetectRange;
    float TransitionTimer;
    bool HasEntered;

    Lancer() :
        ChargeDetectRange{0,0},
		TransitionTimer{0.4f},
        HasEntered{false}
    {}
};
