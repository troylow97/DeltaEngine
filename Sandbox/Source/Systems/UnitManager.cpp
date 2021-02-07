/**********************************************************************************
* \file   UnitManager.cpp
* \brief  The file contains the system for getting specific unit's ID
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "UnitManager.h"

#include "Core/GlobalStruct.h"

namespace DeltaEngine
{
  EntityID UnitManager::player;

  EntityID UnitManager::GetPlayerID()
  {
    env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& id, Player& p, RigidBody& r, State& s, Animator& a)
    {
      if(p.IsJumping)
      {
	      if(r.Velocity.y > 0)
	      {
		      
	      }
      }
      player = id;
    });
  	
    return player;
  }
}
