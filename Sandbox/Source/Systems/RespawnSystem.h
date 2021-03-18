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
    std::vector<Vector3> m_respawns;
  };

  DEFINE_SYSTEM(RespawnSystem, Player)
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    static void CreateCheckpoints(int level);

  private:
    static bool in_tutorial;
    static bool in_level_1;
    static RespawnPoints respawns;
    float dying_countdown = 0.0f;
    void CheckpointsLightUp();
    void Respawning();
    void DeathEffect();
  END_DEFINE_SYSTEM(RespawnSystem)
}
