#pragma once

struct Fiddler
{
    float DurationBeforeExitState;
    float FacePlayerTimer;
    float AttackDelay;
    bool hasAttacked;

    Fiddler() :
        DurationBeforeExitState{ 1.0f },
        FacePlayerTimer{ 2.0f },
        AttackDelay{ 1.0f },
        hasAttacked{ 0 }
    {}
};


