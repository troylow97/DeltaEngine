/**********************************************************************************
* \file   RenderSystem.h
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

#include "ECS/ECSModule.h"
#include "Components/Transform.h"

namespace DeltaEngine
{
  DEFINE_SYSTEM(RenderSystem, Transform)
    void Update() override;
    void LateUpdate() override;
  END_DEFINE_SYSTEM(RenderSystem)
}
