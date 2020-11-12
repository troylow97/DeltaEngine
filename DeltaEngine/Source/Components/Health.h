#pragma once

struct Health
{
	int CurrentHealth;
	int MaxHealth;

	Health() :
		CurrentHealth{10},
		MaxHealth{10}
	{}
};