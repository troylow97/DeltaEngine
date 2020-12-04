#pragma once

#include <Core\Math\Vector.h>

namespace DeltaEngine
{
	struct Player
	{
		int RespawnPoint;
		bool isDead;
		bool isJumping;
		bool isDashing;
		bool startDashingTimer;
		bool allowDashing;
		bool DashDirectionRight;
		float dashingTimerDuration; // max
		float dashingTimerCooldown; // timer

		Player() :
			RespawnPoint(0),
			isDead{false},
			isJumping{false},
			isDashing{false},
			startDashingTimer{false},
			allowDashing{true},
			dashingTimerDuration{1.0f},
			dashingTimerCooldown{1.0f}
		{}
	};


		
}