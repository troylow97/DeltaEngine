/**********************************************************************************
* \file   MouseCalculation.h
* \brief  The file contains the system for doing the calculations for mouse related
*         attacks
* \author Chin, Clara,   100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "ECS/ECSModule.h"
#include "Core/GlobalStruct.h"

namespace DeltaEngine
{
  namespace MouseCalculation
  {
    bool ShootRight();
    bool ShootLeft();
    bool IsMouseOnRight();
    bool IsMouseOnTop();
    bool IsWithinRange(bool right);
    //float CalculateAngleForSMG(bool facing_right);
    Vector2 CalculateGameCoordinate();
    Vector2 CalculateScreenCoordinate();
    Vector2 CalculateDirectionVector();
    Vector2 CalculateDirectionVectorToShoot();
    void FlipShooting();
  }
}
