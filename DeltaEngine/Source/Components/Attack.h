#pragma once

struct Attack
{
	int RangedDamage;
	int MeleeDamage;
	float MaxCooldown;
	float CooldownTimer;
	bool RangeAttack;
	bool MeleeAttack;

	Attack() :
		MeleeDamage{1},
		RangedDamage{1},
		MaxCooldown{0.2f},
		CooldownTimer{0.0f},
		RangeAttack{false},
		MeleeAttack{false}
	{}
};