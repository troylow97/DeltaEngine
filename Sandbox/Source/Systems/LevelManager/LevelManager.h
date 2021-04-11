/**********************************************************************************
* \file   LevelManager.h
* \brief  The file contains the system for gameplay attack
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "ECS/ECSModule.h"

enum class LevelType : unsigned
{
  MainMenu,
  Tutorial,
  Level1
};

namespace DeltaEngine
{
DEFINE_SYSTEM(LevelManager, Player)
  void Update() override;
  void LateUpdate() override;
private:
END_DEFINE_SYSTEM(LevelManager)
}
