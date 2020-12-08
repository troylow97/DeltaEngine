/**********************************************************************************
* \file   UI.h
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
#include "DEpch.h"
#include "Render/Font.h"
#include "Core/Math/Color.h"

namespace DeltaEngine
{
  enum class UIType : unsigned
  {
    Screen,
    Interface,
    Button,
  	Slider,
  	Healthbar,
  	Healthbar_base
  };

  struct UI
  {
    std::string functor_key;
    UIType ui_type;
    unsigned screen;
    int target_screen{-1};
    int previous_screen{-1};
    bool overlay { false };
  };
}
