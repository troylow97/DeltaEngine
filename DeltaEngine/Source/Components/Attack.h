#pragma once

struct Attack
{
  int RangedDamage;
  int MeleeDamage;
  int MeleeComboDamage;
  int NumberOfCombos;
  int MaxComboNumber;
  float MaxCooldown;
  float CooldownTimer;
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
    MaxCooldown{0.2f},
    CooldownTimer{0.0f},
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
