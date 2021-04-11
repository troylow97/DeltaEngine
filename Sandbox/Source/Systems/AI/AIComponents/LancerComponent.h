/**********************************************************************************
* \file   LancerComponent.h
* \brief  The file contains the components for serpentipede
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "Core/Math/Vector.h"

struct Lancer
{
  DeltaEngine::Vector2 ChargeDetectRange;
  float TransitionTimer;
  bool HasEntered;
  
  Lancer() :
    ChargeDetectRange{0,0},
  	TransitionTimer{0.4f},
    HasEntered{false}
  {}
};
