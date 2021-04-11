/**********************************************************************************
* \file   Menus.h
* \brief  The file contains the definition for the game menus
* \author Tan Tong Wee     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "DeltaEngine.h"

namespace DeltaEngine
{
DEFINE_SYSTEM( MenuSystem, GUI )
void Update() override;
void LateUpdate() override;
static float fadeTimer;
static bool fadingIn;
static bool fadingOut;
END_DEFINE_SYSTEM( MenuSystem )
}
