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

		Player() :
			RespawnPoint(0),
			isDead{false},
			isJumping{false},
			isDashing{false}
		{}
	};


		
}