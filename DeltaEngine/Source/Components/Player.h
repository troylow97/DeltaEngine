#pragma once

namespace DeltaEngine
{
  struct Player
  {
    int RespawnPoint;
    int EnemiesDefeated;
    int UpgradePoints;
    int PointsNeededForUpgrade;
    int AttackUpgradeIncrease;
    int HealthUpgradeIncrease;
    float DashingTimerDuration; // max
    float DashingTimerCooldown; // timer
    bool IsDead;
    bool IsJumping;
    bool IsDashing;
    bool StartDashingTimer;
    bool AllowDashing;
    bool DashDirectionRight;
    bool UpgradeAtk;
    bool UpgradeHP;
    bool UpgradedAtk;
    bool UpgradedHP;
    
    Player() :
    	RespawnPoint(0),
        EnemiesDefeated{0},
        UpgradePoints{50},
        PointsNeededForUpgrade{50},
        AttackUpgradeIncrease{1},
        HealthUpgradeIncrease{2},
        DashingTimerDuration{1.0f},
        DashingTimerCooldown{1.0f},
    	IsDead{false},
    	IsJumping{false},
    	IsDashing{false},
    	StartDashingTimer{false},
    	AllowDashing{true},
        DashDirectionRight{true},
        UpgradeAtk{false},
        UpgradeHP{false},
        UpgradedAtk{false},
        UpgradedHP{false}
    {}
  };	
}