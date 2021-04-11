/**********************************************************************************
* \file   UnitManager.h
* \brief  The file contains the system for getting specific unit's ID
* \author Low, Troy,     100% Code Contribution
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
  class UnitManager
  {
    static EntityID player;

  public:
    static void Update();
    static void Reset();
    static EntityID GetPlayerID();
  };

  //DEFINE_SYSTEM(UnitManager, Player)
  //  void UnitManager::Update() override;
  //  static EntityID GetPlayerID();
  //private:
  //  static EntityID playerID;
  //	
  //END_DEFINE_SYSTEM(UnitManager)
}
