/**********************************************************************************
* \file   Manifold.cpp
* \brief  The file contains the class for manifold
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "Core/Math/Vector.h"
#include "Components/Collider.h"

namespace DeltaEngine
{
  struct Manifold
  {
    float interTime;
    float penetration;
    Vector2 normal;

    Manifold::Manifold() :
      interTime{0.0f},
      penetration{0.0f}
    {
    }
  };
}
