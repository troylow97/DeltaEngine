#pragma once

#include <Core\Math\Vector.h>

namespace DeltaEngine
{
	struct Player
	{
		int RespawnPoint;
		bool isDead = false;
		bool isJumping;
		bool isDashing;
		bool startDashingTimer;
		bool allowDashing;
		float dashingTimerDuration; // max
		float dashingTimerCooldown; // timer

		Player() :
			RespawnPoint(0),
			isDead{false},
			isJumping{false},
			isDashing{false},
			startDashingTimer{false},
			allowDashing{false},
			dashingTimerDuration{1.0f},
			dashingTimerCooldown{1.0f}
		{}
	};


		
}