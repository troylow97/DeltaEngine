/**********************************************************************************
* \file   Lifespan.h
* \brief  The file contains components related to Lifespan
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

struct Lifespan
{
  float Timer;

  Lifespan() :
    Timer{1.0f}
  {}

  Lifespan(float time) :
      Timer{ time }
  {}
};
