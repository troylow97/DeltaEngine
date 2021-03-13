/**********************************************************************************
* \file   Player.h
* \brief  The file contains components related to Player
* \author Chin, Clara,   50% Code Contribution
* \author Low, Troy,     50% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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
    float FadingTimer;
    float FadingCountdown;
    bool IsDead;
    bool IsRunning;
    bool IsJumping;
    bool IsDashing;
    bool IsDodging;
    bool IsPunching;
    bool IsShooting;
    bool IsBlocking;
    bool AllowRunning;
    bool AllowPunching;
    bool AllowJumping;
    bool AllowShooting;
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
      FadingTimer{1.0f},
      FadingCountdown{1.0f},
      IsDead{false},
      IsRunning{false},
      IsJumping{false},
      IsDashing{false},
      IsDodging{false},
      IsPunching{false},
      IsShooting{false},
      IsBlocking{false},
      AllowRunning{true},
      AllowPunching{true},
      AllowJumping{true},
      AllowShooting{true},
      StartDashingTimer{false},
      AllowDashing{true},
      DashDirectionRight{true},
      UpgradeAtk{false},
      UpgradeHP{false},
      UpgradedAtk{false},
      UpgradedHP{false}
    {
    }
  };
}
