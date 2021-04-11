/**********************************************************************************
* \file   FiddlerComponent.h
* \brief  The file contains the components for serpentipede
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

struct Fiddler
{
  float DurationBeforeExitState;
  float FacePlayerTimer;
  float AttackDelay;
  bool hasAttacked;
  
  Fiddler() :
     DurationBeforeExitState{ 1.0f },
     FacePlayerTimer{ 2.0f },
     AttackDelay{ 1.0f },
     hasAttacked{ 0 }
  {}
};


