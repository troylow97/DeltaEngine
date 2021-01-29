/**********************************************************************************
* \file   UI.h
* \brief  The file contains components related for UIType
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

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
    native::string32 functor_key;
    UIType ui_type{UIType::Screen};
    unsigned screen{0};
    int target_screen{-1};
    int previous_screen{-1};
    bool overlay{false};
  };
}
