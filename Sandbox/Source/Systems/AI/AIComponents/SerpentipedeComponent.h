#pragma once


struct Serpentipede
{
    float CooldownTimer;
    float BurrowDownDuration;
    float BurrowUpDuration;
    float BurrowDownDelay;
    unsigned int BurrowState; //0: Seen, 1: Burrowing Down, 2: Hidden, 3: Burrowing Up
    int CurrentPoint;
    bool Attacking;

    Serpentipede() :
        CooldownTimer{false},
        BurrowDownDuration{1.0f},
        BurrowUpDuration{2.0f},
        BurrowDownDelay{0.4f},
        BurrowState{0},
		CurrentPoint{0},
        Attacking{false}
	{}
};


