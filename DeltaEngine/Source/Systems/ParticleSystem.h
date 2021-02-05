/**********************************************************************************
* \file   RenderSystem.h
* \brief  The file contains interface of RenderSystem.
* \author Ong, Graeme,   100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "ECS/ECSModule.h"
#include "Components/Transform.h"
#include "Components/ParticleEmitter.h"

namespace DeltaEngine
{
  DEFINE_SYSTEM(ParticleSystem, Transform, ParticleEmitter)
    void Update() override;
  void LateUpdate() override;
  END_DEFINE_SYSTEM(ParticleSystem)
}
