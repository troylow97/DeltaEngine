#pragma once

struct Attack
{
	float Damage;
	float MaxCooldown;
	float CooldownTimer;
	bool isAttacking;

	Attack() :
		Damage{1.0f},
		MaxCooldown{1.0f},
		CooldownTimer{0.0f},
		isAttacking{false}
	{}
};