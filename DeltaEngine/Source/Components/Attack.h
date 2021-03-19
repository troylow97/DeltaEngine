/**********************************************************************************
* \file   Attack.h
* \brief  The file contains the components related to Attack
* \author Chin, Clara,   50% Code Contribution
* \author Low, Troy,     50% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

struct Attack
{
  int RangedDamage;
  int MeleeDamage;
  int SMGDamage;
  int MeleeComboDamage;
  int NumberOfCombos;
  int MaxComboNumber;
  float MeleeCooldown;
  float RangeCooldown;
  float SMGCooldown;
  float MeleeCooldownTimer;
  float RangeCooldownTimer;
  float ComboDuration;
  float ComboCooldownTimer;
  float KnockbackAmount;
  float KnockbackComboAmount;
  float SMGFireRate;
  float AttackDelay;
  float CurrentDodgeCooldown;
  float DodgeCooldown;
  bool RangeAttack;
  bool MeleeAttack;
  bool SMGAttack;
  bool AllowSMGAttack;
  bool Blocking;
  bool StartSMGCooldownTimer;
  bool StartComboCooldownTimer;
  bool DamageEnemy;

  Attack() :
    RangedDamage{1},
    MeleeDamage{1},
    SMGDamage{1},
    MeleeComboDamage{3},
    NumberOfCombos{-1},
    MaxComboNumber{5},
    MeleeCooldown{1.0f},
    RangeCooldown{1.0f},
    SMGCooldown{0.18f},
    MeleeCooldownTimer{-0.1f},
    RangeCooldownTimer{-0.1f},
    ComboDuration{0.4f},
    ComboCooldownTimer{ -0.1f },
    KnockbackAmount{0.0f},
    KnockbackComboAmount{0.0f},
    SMGFireRate{0.18f},
    AttackDelay{-0.2f},
    CurrentDodgeCooldown{-0.1f},
	DodgeCooldown{1.0f},
    RangeAttack{false},
    MeleeAttack{false},
    SMGAttack{false},
    AllowSMGAttack{true},
    Blocking{false},
    StartSMGCooldownTimer{false},
    StartComboCooldownTimer{false},
	DamageEnemy{false}
  {
  }
};
