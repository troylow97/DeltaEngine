/**********************************************************************************
* \file   Attack.h
* \brief  The file contains BLAHBLAHBLAH
* \author Chin, Clara,   X% Code Contribution
* \author Low, Troy,     X% Code Contribution
* \author Ong, Graeme,   X% Code Contribution
* \author Tan, Tong Wee, X% Code Contribution
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
  int MeleeComboDamage;
  int NumberOfCombos;
  int MaxComboNumber;
  float MeleeCooldown;
  float RangeCooldown;
  float MeleeCooldownTimer;
  float RangeCooldownTimer;
  float ComboDuration;
  float ComboCooldownTimer;
  float KnockbackAmount;
  float KnockbackComboAmount;
  bool RangeAttack;
  bool MeleeAttack;
  bool StartComboCooldownTimer;

  Attack() :
    RangedDamage{1},
    MeleeDamage{1},
    MeleeComboDamage{ 3 }, 
    NumberOfCombos{0},
    MaxComboNumber{3},
    MeleeCooldown{1.0f},
	RangeCooldown{1.0f},
    MeleeCooldownTimer{0.0f},
    RangeCooldownTimer{ 0.0f },
    ComboDuration{1.5f},
    ComboCooldownTimer{0.0f},
    KnockbackAmount{0.0f},
    KnockbackComboAmount{0.0f},
    RangeAttack{false},
    MeleeAttack{false},
    StartComboCooldownTimer{false}
  {
  }
};
