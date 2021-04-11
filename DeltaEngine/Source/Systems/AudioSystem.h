/**********************************************************************************
* \file   AudioSystem.h
* \brief  The file contains the implementation for AudioSystem
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
DEFINE_SYSTEM( AudioSystem, AudioSource )
void Update() override;
void LateUpdate() override;
END_DEFINE_SYSTEM( AudioSystem )
}
