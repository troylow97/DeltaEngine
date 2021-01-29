/**********************************************************************************
* \file   AI.h
* \brief  The file contains the components related to AI
* \author Low, Troy,     100% Code Contribution
* 
* 
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include <string>
#include "Core/Utils/NativeString.h"
#include "Core/Math/Vector.h"

struct AI
{
  DeltaEngine::Vector2 original_point;
  native::string32 key;
  native::string32 transition;
  float timer;

  AI() :
    original_point{{0, 0}},
    key("idle"),
    transition("null"),
    timer{0.0f}
  {
  }
};
