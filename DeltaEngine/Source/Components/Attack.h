#pragma once

struct Attack
{
  int RangedDamage;
  int MeleeDamage;
  float MaxCooldown;
  float CooldownTimer;
  bool RangeAttack;
  bool MeleeAttack;

  Attack() :
    RangedDamage{1},
    MeleeDamage{1},
    MaxCooldown{0.2f},
    CooldownTimer{0.0f},
    RangeAttack{false},
    MeleeAttack{false}
  {
  }
};
