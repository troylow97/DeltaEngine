#pragma once
#include "Collider.h"
#include "Core/Math/Vector.h"

namespace DeltaEngine
{
    struct Manifold
    {
        Collider A;
        Collider B;
        float interTime;
        float penetration;
        Vector2 normal;

        Manifold::Manifold(Collider& c1,Collider& c2) :
            A{c1},
            B{c2},
            interTime{0.0f},
            penetration{0.0f}
        {}


    };
}
