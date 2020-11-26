#pragma once

struct Health
{
  int CurrentHealth;
  int MaxHealth;
  bool isInvulnerable;

  Health() :
    CurrentHealth{10},
    MaxHealth{10},
    isInvulnerable{false}
  {}
};
