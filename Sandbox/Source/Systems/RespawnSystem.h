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