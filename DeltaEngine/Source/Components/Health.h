#pragma once

struct Health
{
	int CurrentHealth;
	int MaxHealth;

	Health() :
		CurrentHealth{200},
		MaxHealth{200}
	{}
};