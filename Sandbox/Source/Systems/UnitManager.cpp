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
#include "Core/GameClock/EngineClock.h"
namespace DeltaEngine
{
  EntityID UnitManager::player;

  EntityID UnitManager::GetPlayerID()
  {
    env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& id,Collider& c, Player& p, RigidBody& r, State& s, Animator& a, Health& hp)
    {
      const int velocity = static_cast<int>(r.Velocity.y);
    	
      s.SetFloat("VelocityY", static_cast<float>(velocity));
    
      static float jump = 0;
      static bool fall = false;
      s.SetFloat("VelocityY", static_cast<float>(velocity));
      if (velocity > 0)
      {
        s.SetBool("Jump", true);
      }
      if (s.GetBool("Jump"))
      {
        if (velocity < 0)
        {
          fall = true;
          jump += static_cast<float>(GetEnv().pClock->FixedDeltaTime());
          s.SetFloat("Jump", jump);
        }
      
        if (fall)
        {
          if (c.isCollidingOnFloor)
          {
            s.SetBool("Jump", false);
            s.SetBool("VelocityY", true);
            p.IsJumping = false;
            fall = false;
            jump = 0;
          }
        }
      }
      else if (s.GetBool("VelocityY")) // recovering
      {
        if (a.m_ClipKey == "Clip/DAVE_LAND" && a.LoopsCompleted())
        {
          s.SetBool("VelocityY", false);
          s.SetBool("IsIdle", true);
        }
      }

      if(c.isCollidingOnFloor && a.m_ClipKey == "Clip/DAVE_FALL")
      {
          s.SetBool("Jump", false);
          s.SetBool("VelocityY", true);
          p.IsJumping = false;
          fall = false;
          jump = 0;

      }
    	
      player = id;
    });
  	
    return player;
  }
}
