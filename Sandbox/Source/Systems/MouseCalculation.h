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
    Vector2 CalculateDirectionVector();
    Vector2 CalculateScreenCoordinate();
  }
}
