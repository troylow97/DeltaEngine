#pragma once

namespace DeltaEngine
{
  struct Player
  {
    int RespawnPoint;
    int EnemiesDefeated;
    int UpgradePoints;
    float DashingTimerDuration; // max
    float DashingTimerCooldown; // timer
    bool IsDead;
    bool IsJumping;
    bool IsDashing;
    bool StartDashingTimer;
    bool AllowDashing;
    bool DashDirectionRight;
    
    Player() :
    	RespawnPoint(0),
        EnemiesDefeated{0},
        UpgradePoints{50},
    	IsDead{false},
    	IsJumping{false},
    	IsDashing{false},
    	StartDashingTimer{false},
    	AllowDashing{true},
    	DashingTimerDuration{1.0f},
    	DashingTimerCooldown{1.0f}
    {}
  };	
}