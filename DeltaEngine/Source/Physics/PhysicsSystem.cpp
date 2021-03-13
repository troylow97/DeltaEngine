/**********************************************************************************
* \file   PhysicsSystem.cpp
* \brief  The file contains logic for physics system
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "PhysicsSystem.h"
#include "Core/GlobalStruct.h"
#include "Core/GameClock/EngineClock.h"
#include "Collision.h"

#include "Audio/AudioEngine.h"

namespace DeltaEngine
{
  void PhysicsSystem::Initialize()
  {
    m_gravity_amount = {0, -70.0f};
    CurrentJumpTicks = 0;
    MaxJumpTicks = 8;
    CurrentDashTicks = 0;
    MaxDashTicks = 8;
    InitialJumpForce = 800.0f;
    JumpForce = InitialJumpForce;
  }

  void PhysicsSystem::SetBounds(RigidBody& r1)
  {
    //Check for infinite-mass
    if (r1.Mass <= 0)
    {
      r1.Mass = 1.0f;
    }

    if (r1.FrictionCoeff <= 0)
    {
      r1.FrictionCoeff = 0.01f;
    }
    else if (r1.FrictionCoeff > 4)
    {
      r1.FrictionCoeff = 4.0f;
    }
  }

  void PhysicsSystem::Update()
  {
    for (size_t step = 0; step < env.pClock->Timesteps(); ++step)
      UpdateVelocity();
  }

  void PhysicsSystem::LateUpdate()
  {
  }

  void PhysicsSystem::UpdateVelocity()
  {
    em.ForEach([&](EntityID id1, RigidBody& r1, Transform& t1, Collider& c1)
    {
      if (r1.isMoveable)
      {
        SetBounds(r1);

        //Set Euler
        t1.position += r1.Velocity * env.pClock->FixedDeltaTime();

        //Dash and Jump
        if (em.HasComponent<Player>(id1))
        {
          auto& player = em.GetComponent<Player>(id1);
          UpdateJumpAndDashDelay(player);
          Dash(player, r1, c1);
          Jump(player, r1, c1);
          //Player Movement
          if (!player.IsDashing && static_cast<int>(r1.Direction.x) != 0)
          {
              if (c1.isCollidingOnFloor)
              {
                  const Vector2 move = (r1.Direction * r1.Movespeed) + (r1.Direction * r1.InherentAcceleration * r1.AccelerationPickup);
                  r1.AccumulatedForce += move * r1.Mass * 0.5f;
              }
              else
              {
                  const float move = (r1.Direction.x * r1.Movespeed);
                  r1.AccumulatedForce.x += move * r1.Mass;
              }
          }
          else
          {
              r1.InherentAcceleration = 0;
          }
        }
        else
        {
            const Vector2 move = (r1.Direction * r1.Movespeed);
            r1.AccumulatedForce += move * r1.Mass;
        }




        //Apply Gravity
        if (r1.hasGravity && !c1.isCollidingOnFloor)
        {
          if (!em.HasComponent<Player>(id1))
            r1.Acceleration = m_gravity_amount;
        }
        else
        {
          r1.Acceleration = {0, 0};
        }

        //Apply Acceleration
        const Vector2 newAcceleration = r1.AccumulatedForce * (1 / r1.Mass) + r1.Acceleration;
        r1.Velocity += newAcceleration * env.pClock->FixedDeltaTime();

      	if(em.HasComponent<Player>(id1))
      	{
            //Apply Friction -> when no input
            if (c1.isCollidingOnFloor && static_cast<int>(r1.Direction.x) == 0)
            {
                const float dragForceMagnitude = (r1.Velocity.Magnitude() * r1.FrictionCoeff);
                const Vector2 dragForceVector = (dragForceMagnitude * -(Normalise(r1.Velocity))) * env.pClock->FixedDeltaTime();
                r1.Velocity *= 0.8f;
            }
            else //kinetic one
            {
                if (c1.isCollidingOnFloor)
                {
                    const float dragForceMagnitude = (r1.Velocity.Magnitude() * r1.FrictionCoeff);
                    const Vector2 dragForceVector = (0.5f * dragForceMagnitude * -(Normalise(r1.Velocity))) * env.pClock->FixedDeltaTime();
                    r1.Velocity += dragForceVector;
                }
                else
                {
                    const float dragForceMagnitude = (r1.Velocity.Magnitude() * r1.FrictionCoeff);
                    const Vector2 dragForceVector = (0.1f * dragForceMagnitude * -(Normalise(r1.Velocity))) * env.pClock->FixedDeltaTime();
                    r1.Velocity += dragForceVector;
                }
            }

      	}

        //Apply Soft Drag
        r1.Velocity *= 0.96f;

        r1.AccumulatedForce = {0, 0};
        c1.isCollidingOnFloor = false;
      }
      else
        r1.Velocity = {0, 0}; //If non-moveable
    });
  }

  void PhysicsSystem::Dash(Player& p, RigidBody& r, Collider& c)
  {
    //Jumping,Dashing
	// if ()
    {
      if (p.IsJumping && c.isCollidingOnFloor)
      {
        CurrentJumpTicks = 1;
        JumpForce = InitialJumpForce;
      }
      //else if (p.IsDashing && CurrentDashTicks < MaxDashTicks)
      //{
      //  CurrentDashTicks++;
      //  p.AllowPunching = false;
      //  p.AllowShooting = false;
      //  if (p.DashDirectionRight)
      //  {
      //    r.AccumulatedForce += Vector2{5000 + r.Mass * 100, 0};
      //  }
      //  else
      //  {
      //    r.AccumulatedForce -= Vector2{5000 + r.Mass * 100, 0};
      //  }
      //}
      else if (p.IsDodging && CurrentDashTicks < MaxDashTicks)
      {
        CurrentDashTicks++;
        p.AllowPunching = false;
        p.AllowShooting = false;
        p.AllowRunning = false;
      	if(c.isCollidingOnFloor)
      	{
            if (p.DashDirectionRight)
                r.AccumulatedForce += Vector2{ 3000 + r.Mass * 100, 0 };
            else
                r.AccumulatedForce -= Vector2{ 3000 + r.Mass * 100, 0 };
      	}
        else //dashing in mid air
        {
            if (p.DashDirectionRight)
                r.AccumulatedForce += Vector2{ 800 + r.Mass * 30, 0 };
            else
                r.AccumulatedForce -= Vector2{ 800 + r.Mass * 30, 0 };
        }

      }
    }
  	
    //if (p.IsDashing)
    //{
    //  CurrentDashTicks = 0;
    //  p.IsDashing = false;
    //}
    //else if (p.IsDodging)
    //{
    //  CurrentDashTicks = 0;
    //  p.IsDodging = false;
    //}

    if (CurrentDashTicks >= MaxDashTicks)
    {
      //p.IsDashing = false;
      p.IsDodging = false;
      CurrentDashTicks = 0;
      DashDelay = 0.5f;
    }
  }

  void PhysicsSystem::Jump(Player& p, RigidBody& r, Collider& c)
  {
    if (p.IsJumping && CurrentJumpTicks >= 1)
    {
      r.AccumulatedForce += Vector2{ 0, JumpForce + r.Mass * 100 };
      JumpForce *= 0.60f;
      
      if (CurrentJumpTicks < MaxJumpTicks)
      {
        CurrentJumpTicks++;
        p.AllowPunching = false;
        p.AllowShooting = false;
        p.AllowRunning = false;
      }
      else
      {
        p.AllowRunning = true;
        p.AllowPunching = true;
        p.IsJumping = false;
        JumpForce = InitialJumpForce;
        CurrentJumpTicks = 0;
        JumpDelay = 0.5f;
      }
    }
    
    //Apply Gravity for player
    if (r.hasGravity && !c.isCollidingOnFloor && !p.IsDashing && !p.IsDodging)
      r.Acceleration = m_gravity_amount;
    else
      r.Acceleration = { 0, 0 };
  }
  void PhysicsSystem::UpdateJumpAndDashDelay(Player& p)
  {
    if (DashDelay > 0.0f)
      DashDelay -= env.pClock->FixedDeltaTime();
    else
    {
      p.AllowRunning = true;
      p.AllowPunching = true;
      p.AllowShooting = true;
    }

    if (JumpDelay > 0.0f)
        JumpDelay -= env.pClock->FixedDeltaTime();
    else
    {
      p.AllowRunning = true;
      p.AllowPunching = true;
      p.AllowShooting = true;
    } 	
  }
}
