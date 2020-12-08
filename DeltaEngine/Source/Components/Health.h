/**********************************************************************************
* \file   Health.h
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
