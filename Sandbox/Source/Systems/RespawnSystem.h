/**********************************************************************************
* \file   RespawnSystem.h
* \brief  The file contains the system for respawning the player
* \author Chin, Clara,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "ECS/ECSModule.h"

namespace DeltaEngine
{
  struct RespawnPoints
  {
    std::vector<Vector2> m_respawns;
  };

  DEFINE_SYSTEM(RespawnSystem, Player)
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
  private:
    RespawnPoints respawns;
  END_DEFINE_SYSTEM(RespawnSystem)
}
