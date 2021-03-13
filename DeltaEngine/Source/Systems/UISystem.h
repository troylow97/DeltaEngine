/**********************************************************************************
* \file   UISystem.h
* \brief  The file contains the system for updating and displaying UI in the game
* \author Chin, Clara,     70% Code Contribution
* \author Low, Troy,       30% Code Contribution
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
DEFINE_SYSTEM( UISystem, GUI)
void Initialize() override;
void Update() override;
void LateUpdate() override;

static void PopScreen();
static void PushScreen(unsigned screen);

END_DEFINE_SYSTEM( UISystem )
}
