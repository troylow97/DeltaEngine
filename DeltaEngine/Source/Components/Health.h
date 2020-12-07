#pragma once

struct Health
{
  int CurrentHealth;
  int MaxHealth;
  float isDamagedTimer;
  bool isInvulnerable;

  Health() :
    CurrentHealth{10},
    MaxHealth{10},
    isDamagedTimer{0.0f},
    isInvulnerable{false}
  {}
};
