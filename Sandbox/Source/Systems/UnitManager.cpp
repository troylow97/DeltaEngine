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
    env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID& id,Collider& c, Player& p, RigidBody& r, State& s, Animator& a)
    {
      static float jump = 0;
      static bool fall = false;
      s.SetFloat("VelocityY", r.Velocity.y);
      if (r.Velocity.y)
      {
        s.SetBool("Jump", true);
      }
      if (s.GetBool("Jump"))
      {
        if (r.Velocity.y < 0)
        {
          fall = true;
          jump += static_cast<float>(GetEnv().pClock->FixedDeltaTime());
          s.SetFloat("Jump", jump);
        }

        if (fall)
        {
          std::cerr << "fall" << std::endl;
          if (c.isCollidingOnFloor)
          {
            std::cerr << "land" << std::endl;
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
        std::cerr << "recovering" << a.LoopsCompleted() << ' ' << a.m_ClipKey << std::endl;
        if (a.m_ClipKey == "Clip/DAVE_LAND" && a.LoopsCompleted())
        {
          std::cerr << "recovered" << std::endl;
          s.SetBool("IsIdle", true);
          s.SetBool("VelocityY", false);
        }
      }

      player = id;
    });
  	
    return player;
  }
}
