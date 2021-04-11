/**********************************************************************************
* \file   GUISystem.h
* \brief  The file contains the definition for the GUISystem
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
DEFINE_SYSTEM( GUISystem, GUI )
void Update() override;
void LateUpdate() override;

static void Attacked(bool b);
static void Heal(bool b);
END_DEFINE_SYSTEM( GUISystem )
}
